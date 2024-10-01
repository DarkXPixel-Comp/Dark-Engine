#include "LaunchEngineLoop.h"
#include "CoreGlobals.h"
#include "RHI.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Application/UIApplication.h"
#include "Engine/EditorEngine.h"
#include "Rendering/UIRHIRenderer.h"
#include "Misc/ProcessManager.h"
#include "Widgets/UIMainMenuBar.h"
#include "UICore.h"
#include "Misc/Paths.h"
#include "Widgets/UIWidgetTest.h"
#include "Widgets/UIDock.h"
#include "Widgets/UILogs.h"
#include "Logging/LogMacros.h"
#include "Widgets/UIEditorViewport.h"
#include "Widgets/UIEditorSettings.h"
#include "Widgets/UIConsole.h"
#include "Widgets/UIWorldOutliner.h"
#include "Widgets/UIEntityProperties.h"
#include "Widgets/UIGPUMemoryStats.h"
#include "Widgets/UIWorldSettings.h"
#include "Widgets/UIProfiler.h"
#include "Shader.h"
#include "ShaderCompiler.h"
#include "Console/GlobalInputOutConsole.h"
#include "Misc/Config.h"
#include "GlobalResource.h"
#include "GameTimer.h"
#include "optick.h"
#include "Object.h"
#include "Python.h"
#include "SceneResourceBuilder.h"
#include "SceneResourceImporter.h"
#include "PhysicsCore.h"


//#include "lua.hpp"
#include "sol2/sol.hpp"
#include <iostream>
#include <filesystem>



const char* HelloWorld()
{
	return "Hello, World";
}



DECLARE_LOG_CATEGORY(Launch, Display);






struct FEditorLayout
{
	void SetupEditorLayout(UIWindow* InRootWindow)
	{
		RootWindow = InRootWindow;

		MainDock = MakeShareble(new UIDock());
		RootWindow->AddWidget(MainDock);


		TSharedPtr<UIConsole> Console = MakeShareble(new UIConsole());
		MainDock->AddChild(Console);
		//Console->SetName(TEXT("Console"));
		/*TSharedPtr<UILogs> Logs = MakeShareble(new UILogs());
		MainDock->AddChild(Logs);
		Logs->SetName(TEXT("Logs"));*/
		//Logs->bHaveCloseButton = true;
		

		MainMenuBar = MakeShareble(new UIMainMenuBar());
		MainDock->SetMainMenuBar(MainMenuBar);
		//MainMenuBar->SetTitle(TEXT("Dark Engine"));

		TSharedPtr<UIWorldOutliner> WorldOutliner = MakeShareble(new UIWorldOutliner());
		MainDock->AddChild(WorldOutliner);

		TSharedPtr<UIWorldSettings>	WorldSettings = MakeShareble(new UIWorldSettings());
		MainDock->AddChild(WorldSettings);

		TSharedPtr<UIEntityProperties> EntityProperties = MakeShareble(new UIEntityProperties());
		MainDock->AddChild(EntityProperties);



		RootViewport = MakeShareble(new UIEditorViewport());
		{
			RootViewport->SetName(TEXT("EditorViewport"));
			//RootViewport->bHaveCloseButton = true;
			MainDock->AddChild(RootViewport);
		}

		//EditorSettings->bHaveCloseButton = true;

		TSharedPtr<UIMenuItem> ExitButton = MakeShareble(new UIMenuItem());
		ExitButton->SetName(TEXT("Exit"));
		ExitButton->MenuItemDelegate.Bind([=](bool bPressed)
			{
				if (bPressed)
				{
					RequestExit();
				}
			});
		

		TestWidget = MakeShareble(new UIWidgetTest());
		//MainDock->AddChild(TestWidget);

		TSharedPtr<UIMenuItem> CreateSettings = MakeShareble(new UIMenuItem());
		CreateSettings->SetName(TEXT("Settings"));
		CreateSettings->MenuItemDelegate.Bind([&, CreateSettings](bool bPressed)
			{
				if (bPressed)
				{
					TSharedPtr<UIEditorSettings> EditorSettings = MakeShareble(new UIEditorSettings());
					EditorSettings->SetEditorViewport(RootViewport);
					MainDock->AddChild(EditorSettings);
				//	CreateSettings->AddChild(EditorSettings);
				}
				//else
				//{
				//	//CreateSettings->RemoveChild(EditorSettings);
				//	MainDock->RemoveChild(EditorSettings);
				//}
			});

		TSharedPtr<UIMenu> File = MakeShareble(new UIMenu());
		MainMenuBar->AddMenu(File);
		File->SetName(TEXT("File"));
		File->AddChild(CreateSettings);
		File->AddChild(ExitButton);

		//GPUMemoryStats = MakeShareble(new UIGPUMemoryStats());
		//MainDock->AddChild(GPUMemoryStats);


		TSharedPtr<UIMenu> Windows = MakeShareble(new UIMenu());
		Windows->SetName(TEXT("Windows"));
		MainMenuBar->AddMenu(Windows);
		{
			TSharedPtr<UIMenuItem> CreateGPUStats = MakeShareble(new UIMenuItem());
			CreateGPUStats->SetName(TEXT("GPUStats"));
			CreateGPUStats->MenuItemDelegate.Bind([this, CreateGPUStats](bool)
				{
					TSharedPtr<UIGPUMemoryStats> GPUStats = MakeShareble(new UIGPUMemoryStats());
					MainDock->AddChild(GPUStats);
					CreateGPUStats->SetDependWidget(GPUStats);
					//CreateGPUStats->AddChild(GPUStats);
				});
			Windows->AddChild(CreateGPUStats);

			TSharedPtr<UIMenuItem> CreateConsole = MakeShareble(new UIMenuItem());
			CreateConsole->SetName(TEXT("Console"));
			CreateConsole->SetDependWidget(Console);
			CreateConsole->MenuItemDelegate.Bind([CreateConsole, Console, this](bool)
				{
					TSharedPtr<UIConsole> NewConsole = MakeShareble(new UIConsole());
					MainDock->AddChild(NewConsole);
					CreateConsole->SetDependWidget(NewConsole);
				});

			Windows->AddChild(CreateConsole);

			TSharedPtr<UIMenuItem> CreateProfiler = MakeShareble(new UIMenuItem());
			CreateProfiler->SetName(TEXT("Profiler"));
			CreateProfiler->MenuItemDelegate.Bind([this, CreateProfiler](bool)
				{
					TSharedPtr<UIProfiler> Profiler = MakeShareble(new UIProfiler());
					MainDock->AddChild(Profiler);
					CreateProfiler->SetDependWidget(Profiler);
				});
			Windows->AddChild(CreateProfiler);
		}
	
	}

	void Destroy()
	{
		RootViewport.reset();
		MainDock.reset();
		//EditorSettings.reset();
		MainMenuBar.reset();
	}

public:
	UIWindow* RootWindow = nullptr;
	TSharedPtr<UIEditorViewport> RootViewport = nullptr;
	TSharedPtr<UIDock> MainDock = nullptr;
//	TSharedPtr<UIEditorSettings> EditorSettings;
	TSharedPtr<UIMainMenuBar> MainMenuBar;
	//TSharedPtr<UIWorldOutliner>	WorldOutliner;
	//TSharedPtr<UIEntityProperties> EntityProperties;
	TSharedPtr<UIWidgetTest> TestWidget;
	//TSharedPtr<UIGPUMemoryStats> GPUMemoryStats;

} EditorLayout;


int32 Write(lua_State* State)
{
	for (size_t i = 1; i < lua_gettop(State) + 1; ++i)
	{
		//GGlobalConsole.AddLog(lua_tostring(State, i));
		//GGlobalConsole.AddLog((const char*)lua_touserdata(State, i));
		//const void* ptr = lua_topointer(State, i);
		GGlobalConsole.AddLog(lua_tostring(State, i));
	}

	return 0;
}


class Test
{
public:
	void test(std::wstring str)
	{
		GGlobalConsole.AddLog(str);
	}
};

int32 FEngineLoop::TestInit()
{
	return 0;
}


//int HandleLua(lua_State* L)
//{
//	assert(false);
//	return 0;
//}

int LuaHandle1(lua_State*, sol::optional<const std::exception&>, sol::string_view)
{
	assert(false);
	return 0;
}


const FString VitalFolders[] = {"Content", "Config", "Shaders", "bin"};


int32 CheckFiles()
{
	int32 AllFilesIsOk = false;
	int32 NumFiles = 0;

	std::vector<FString> TempFolders(std::begin(VitalFolders), std::end(VitalFolders));

	for (const auto& Entry : std::filesystem::directory_iterator(!FPaths::EngineDir()))
	{
		auto& Path = Entry.path();
		auto str = Path.filename().wstring();

		auto Result = std::find(std::begin(VitalFolders), std::end(VitalFolders), Path.filename().wstring());

		if (Result != std::end(VitalFolders))
		{
			++NumFiles;
			TempFolders.erase(std::find(TempFolders.begin(), TempFolders.end(), *Result));
		}
	}


	for (auto& LostFile : TempFolders)
	{
		GAdditionalErrorMsg += (LostFile + TEXT("\n"));
	}


	return NumFiles == DE_ARRAY_COUNT(VitalFolders);

}

int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	//OPTICK_START_CAPTURE();
	OPTICK_FRAME("PreInit");
	//setlocale(LC_ALL, ".utf-16");

	if (!CheckFiles())
	{
		FPlatformMisc::CreateMessageBoxError(*FString::PrintF(TEXT("Not found all vital files and folders: \n%s"), *GetAdditionalErrorMsg()), TEXT("Error"));
		RequestExit();
		return -1;
	}

	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	FMath::RandInit(time(0));
	//ScriptState.set_panic(HandleLua);
	//ScriptState.set_exception_handler(LuaHandle1);
	ScriptState.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);
//	ScriptState.script_file(!(FPaths::EngineScriptsDir() / TEXT("Test/Engine.script")));
	// TEMP!!!!
	bool bWithOSConsole = FString(CmdLine) == TEXT("-CMD");
	GGlobalConsole.Initialize(bWithOSConsole);
	GGlobalConsole.SetAutoClose(false);

	FConfigCache::InitConfigSystem();

	InitGObject();
	auto Res = ScriptState.load_file(!(FPaths::EngineScriptsDir() / TEXT("Test/Engine.script")));
	if (Res.valid())
	{
		ScriptState.script_file(!(FPaths::EngineScriptsDir() / TEXT("Test/Engine.script")));
	}
	else
	{
		DE_LOG(Launch, Error, TEXT("Error load lua script: %s"), *FString(Res.operator std::string()));
	}


	ScriptState.set_function("print", [](std::wstring str)
		{
			GGlobalConsole.AddLog(str);
		});



	GGlobalConsole.RegisterConsoleVariableRef(TEXT("r.MaxFps"), GMaxFPS, TEXT("Set max fps"))->SetOnChangedCallback([](IConsoleVariable* Var)
		{
			if (Var->GetFloat() <= 0)
			{
				Var->Set(100000.f);
			}
		});

	GGlobalConsole.RegisterConsoleCommand(TEXT("c.CreateOS"), [](const TArray<FString>& Args)
		{
			GGlobalConsole.CreateConsoleOS(Args.GetSize() > 0 ? Args[0] : TEXT("Undefined"));
		}, TEXT("(EXPERIMENTAL)Create Console OS (On close, terminate Engine)"));

	GGlobalConsole.RegisterConsoleCommand(TEXT("c.DestroyOS"), [](const TArray<FString>&)
		{
			GGlobalConsole.DestroyConsole();
		});

	GGlobalConsole.RegisterConsoleCommand(TEXT("g.RequestExit"), [](const TArray<FString>&) 
		{
			RequestExit();
		}, TEXT("Request Exit"));

	GGlobalConsole.RegisterConsoleCommand(TEXT("Suka"), [](const TArray<FString>& Args)
		{
			for (const auto& i : Args)
			{
				GGlobalConsole.AddLog(i);
			}
		}, TEXT("Repeat args"));


	GGlobalConsole.RegisterConsoleVariableRef(TEXT("g.Renderer.RenderMode"), GRenderMode);

	GGlobalConsole.RegisterConsoleVariableRef(TEXT("g.RenderSettings.bFXAA"), GFXAAEnable);

	DE_LOG(Launch, Log, TEXT("FEngineLoop Pre init. Cmd: %s"), CmdLine);

	FShaderTypeRegistration::CommitAll();

	RHIInit();

	DE_LOG(Launch, Log, TEXT("RHI init"));

	if (IsExitRequested())
	{
		return -1;
	}

	CompileGlobalShaders();

	InitGlobalRenderResources();

	InitPhysicsCore();

	Engine = new DEditorEngine();

	DE_LOG(Launch, Log, TEXT("Create Engine"));
	

	TSharedPtr<UIApplication> CurrentApplication = UIApplication::Create();
	DE_LOG(Launch, Log, TEXT("Create UIApplication"));
	TSharedPtr<UIWindow> RootWindow = MakeShareble(new UIWindow());
	DE_LOG(Launch, Log, TEXT("Create UIWindow"));
	TSharedPtr<FUIRHIRenderer> UIRenderer = MakeShareble(new FUIRHIRenderer());
	DE_LOG(Launch, Log, TEXT("Create UIRenderer"));

	CurrentApplication->InitializeRenderer(UIRenderer);
	DE_LOG(Launch, Log, TEXT("Init UIRenderer"));
	RootWindow->SetbWindowBorder(false);
	CurrentApplication->AddWindow(RootWindow);
	DE_LOG(Launch, Log, TEXT("Add window"));

	RootWindow->ShowWindow();
	DE_LOG(Launch, Log, TEXT("Show window"));
	RootWindow->SetTitle("Edtior");
	DE_LOG(Launch, Log, TEXT("Title root window - \"%s\""), *RootWindow->GetTitle());


	EditorLayout.SetupEditorLayout(RootWindow.get());
	
	DE_LOG(Launch, Log, TEXT("Setup EditorLayout"));

	
	GGlobalConsole.RegisterConsoleCommand(TEXT("r.Renderer.SetViewportResolution"),
		[](const TArray<FString>& Args)
		{
			if (Args.Num() >= 2)
			{
				int32 X, Y;
				EditorLayout.RootViewport->GetSceneViewport()->Resize(FString::FromString(X, *Args[0]), 
					FString::FromString(Y, *Args[1]));
			}
		});

	TestInit();

	{

	}


	sol::function PreInitFunc = ScriptState["PreInit"];
	if (PreInitFunc.valid())
	{
		PreInitFunc(!FString(CmdLine));
	}

	return 0;

} 

int32 FEngineLoop::Init()
{
	RHIPostInit();
	DE_LOG(Launch, Log, TEXT("RHI Post init"));
	((DEditorEngine*)Engine)->NewMap();

	//FSceneResourceBuilder::Build(TEXT("Meshes/Cube.fbx"));
	FSceneResourceImporter Importer;
	Importer.ImportFromFile(TEXT("Meshes/Cube.fbx"));
	Importer.GetAllStaticMeshes();

	FGameTimer::Start();
	FGameTimer::Reset();

	GWorld->BeginPlay();

	return 0;
}

void FEngineLoop::Tick()
{
	OPTICK_FRAME("MainThread");
	FGameTimer::Tick();
	float DeltaTime = FGameTimer::DeltaTime();
//	GWorld->FetchPhysic();
	UIApplication::Get()->Tick(DeltaTime);
	Engine->Tick(DeltaTime);

	GWorld->Tick(DeltaTime, true);

	FProcessManager::UpdateProcesses();

	sol::protected_function TickFunc = ScriptState["Tick"];

	sol::error T = TickFunc();
	if (TickFunc.valid())
	{
		sol::protected_function_result Result = TickFunc();
		if (!Result.valid())
		{
			sol::error err = Result;
			std::string what = err.what();

		}
	}
}




void FEngineLoop::Exit()
{
	GWorld->EndPlay();

	FProcessManager::CloseAllProcesses();
	EditorLayout.Destroy();
	UIApplication::Destroy();
	Engine->Shutdown();
	delete Engine;
	Engine = nullptr;

	ShutdownPhysicsCore();

	RHIExit();


	Logger::Exit();

	GGlobalConsole.Destroy();

	//OPTICK_STOP_CAPTURE();
	//OPTICK_SAVE_CAPTURE(TestSave);

	/*OPTICK_STOP_CAPTURE();
	OPTICK_SAVE_CAPTURE(-(FPaths::EngineBinariesDir() + TEXT("ProfilerCaptures/Capture.opt")));*/
	OPTICK_SHUTDOWN();
}