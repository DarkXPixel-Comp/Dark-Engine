#include "LaunchEngineLoop.h"
#include "CoreGlobals.h"
#include "RHI.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Application/UIApplication.h"
#include "Engine/EditorEngine.h"
#include "Rendering/UIRHIRenderer.h"
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
#include "Shader.h"
#include "ShaderCompiler.h"
#include "Console/GlobalInputOutConsole.h"
#include "Misc/Config.h"
#include "GlobalResource.h"
#include "GameTimer.h"

//#include "lua.hpp"
#include "sol2/sol.hpp"
#include <iostream>




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
		Console->SetName(TEXT("Console"));
		/*TSharedPtr<UILogs> Logs = MakeShareble(new UILogs());
		MainDock->AddChild(Logs);
		Logs->SetName(TEXT("Logs"));*/
		//Logs->bHaveCloseButton = true;
		

		MainMenuBar = MakeShareble(new UIMainMenuBar());
		MainDock->SetMainMenuBar(MainMenuBar);
		MainMenuBar->SetTitle(TEXT("Dark Engine"));

		RootViewport = MakeShareble(new UIEditorViewport());
		{
			RootViewport->SetName(TEXT("EditorViewport"));
			//RootViewport->bHaveCloseButton = true;
			MainDock->AddChild(RootViewport);
		}

		EditorSettings = MakeShareble(new UIEditorSettings());
		EditorSettings->SetEditorViewport(RootViewport);
		//ditorSettings->bHaveCloseButton = true;

		TSharedPtr<UIMenuItem> ExitButton = MakeShareble(new UIMenuItem());
		ExitButton->SetName(TEXT("Exit"));
		ExitButton->MenuItemDelegate.Bind([=](bool bPressed)
			{
				if (bPressed)
				{
					RequestExit();
				}
			});
		

		TSharedPtr<UIMenuItem> CreateSettings = MakeShareble(new UIMenuItem());
		CreateSettings->SetName(TEXT("Settings"));
		CreateSettings->MenuItemDelegate.Bind([&, CreateSettings](bool bPressed)
			{
				if (bPressed)
				{
					MainDock->AddChild(EditorSettings);
					CreateSettings->AddChild(EditorSettings);
				}
				else
				{
					CreateSettings->RemoveChild(EditorSettings);
					MainDock->RemoveChild(EditorSettings);
				}
			});

		TSharedPtr<UIMenu> File = MakeShareble(new UIMenu());
		MainMenuBar->AddMenu(File);
		File->SetName(TEXT("File"));
		File->AddChild(CreateSettings);
		File->AddChild(ExitButton);
	
	}

	void Destroy()
	{
		RootViewport.reset();
		MainDock.reset();
		EditorSettings.reset();
		MainMenuBar.reset();
	}

public:
	UIWindow* RootWindow = nullptr;
	TSharedPtr<UIEditorViewport> RootViewport = nullptr;
	TSharedPtr<UIDock> MainDock = nullptr;
	TSharedPtr<UIEditorSettings> EditorSettings;
	TSharedPtr<UIMainMenuBar> MainMenuBar;

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





int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	//setlocale(LC_ALL, ".utf-16");

	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	FMath::RandInit(time(0));
	ScriptState.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);
	ScriptState.script_file(!(FPaths::EngineScriptsDir() / TEXT("Test/Engine.script")));
	ScriptState.set_function("print", [](std::wstring str)
		{
			GGlobalConsole.AddLog(str);
		});

	// TEMP!!!!
	bool bWithOSConsole = FString(CmdLine) == TEXT("-CMD");
	GGlobalConsole.Initialize(bWithOSConsole);
	GGlobalConsole.SetAutoClose(false);

	FConfigCache::InitConfigSystem();

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
					FString::FromString(Y, *Args[0]));
			}
		});

	TestInit();

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


	return 0;
}

void FEngineLoop::Tick()
{
	float DeltaTime = FGameTimer::DeltaTime();
	FGameTimer::Tick();
	UIApplication::Get()->Tick(DeltaTime);
	Engine->Tick(DeltaTime);
	sol::function TickFunc = ScriptState["Tick"];
	if (TickFunc.valid())
	{
		TickFunc();
	}
}

void FEngineLoop::Exit()
{
	EditorLayout.Destroy();
	UIApplication::Destroy();
	Engine->Shutdown();
	delete Engine;
	Engine = nullptr;

	RHIExit();

	Logger::Exit();

	GGlobalConsole.Destroy();
}