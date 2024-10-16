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

#include "GlobalResource.h"
#include "VulkanDynamicRHI.h"





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







int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	//setlocale(LC_ALL, ".utf-16");

	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	FMath::RandInit(time(0));

	// TEMP!!!!
	bool bWithOSConsole = FString(CmdLine) == TEXT("-CMD");
	GGlobalConsole.Initialize(bWithOSConsole);
	GGlobalConsole.SetAutoClose(false);

	GGlobalConsole.RegisterConsoleCommand(TEXT("c.CreateOS"), [](const TArray<FString>& Args)
		{
			GGlobalConsole.CreateConsoleOS(Args.GetSize() > 0 ? Args[0] : TEXT("Undefined"));
		}, TEXT("(EXPERIMENTAL)Create Console OS (On close, terminate Engine)"));

	GGlobalConsole.RegisterConsoleCommand(TEXT("c.DestroyOS"), [](const TArray<FString>&)
		{
			GGlobalConsole.DestroyConsole();
		});

	//GGlobalConsole.RegisterConsoleCommand(TEXT("g.RequestExit"), [](const TArray<FString>&) 
	//	{
	//		RequestExit();
	//	}, TEXT("Request Exit"));

	GGlobalConsole.RegisterConsoleCommand(TEXT("Suka"), [](const TArray<FString>& Args)
		{
			for (const auto& i : Args)
			{
				GGlobalConsole.AddLog(i);
			}
		}, TEXT("Repeat args"));

	GGlobalConsole.RegisterConsoleVariableRef(TEXT("g.Renderer.RenderMode"), GRenderMode);


	DE_LOG(Launch, Log, TEXT("FEngineLoop Pre init (%s)"), CmdLine);

	FShaderTypeRegistration::CommitAll();

	RHIInit();

	DE_LOG(Launch, Log, TEXT("RHI init"));


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
	Engine->Tick(1);
	UIApplication::Get()->Tick();
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