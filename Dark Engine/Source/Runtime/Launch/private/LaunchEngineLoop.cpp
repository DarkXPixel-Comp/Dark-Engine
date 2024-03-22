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



DECLARE_LOG_CATEGORY(Launch, Display);


void SetupEditorLayout(UIWindow* RootWindow)
{
	TSharedPtr<UIWidgetTest> Test = MakeShareble(new UIWidgetTest());


	TSharedPtr<UIDock> MainDock = MakeShareble(new UIDock());

	TSharedPtr<UILogs> Logs = MakeShareble(new UILogs());
	{
		Logs->SetName("Logs");
		MainDock->AddChild(Logs);
	}

	TSharedPtr<UIMainMenuBar> MainMenuBar = MakeShareble(new UIMainMenuBar());
	{
		MainMenuBar->SetSize(FIntPoint(0, 20));
	}


	TSharedPtr<UIViewport> MainViewport = MakeShareble(new UIViewport());
	{
		MainViewport->SetName("UIViewport");
		//MainDock->AddChild(MainViewport);
	}

	TSharedPtr<UIMenu> Menu = MakeShareble(new UIMenu());
	{
		Menu->SetName("Menu");
		MainMenuBar->AddChild(Menu);
	}
	TSharedPtr<UIMenuItem> ExitMenuItem = MakeShareble(new UIMenuItem());
	{
		ExitMenuItem->SetName("Exit");
		ExitMenuItem->MenuItemDelegate.Bind([&]()
			{
				GIsRequestingExit = true;
			});

		Menu->AddChild(ExitMenuItem);
	}

	TSharedPtr<UIEditorViewport> EditorViewport = MakeShareble(new UIEditorViewport());
	{
		EditorViewport->SetName(TEXT("EditorViewport"));
		MainDock->AddChild(EditorViewport);
	}

	TSharedPtr<UIMenuItem> AddNewWindowItem = MakeShareble(new UIMenuItem());
	{
		AddNewWindowItem->SetName("Add new Window");
		AddNewWindowItem->MenuItemDelegate.Bind([=]()
			{
				TSharedPtr<UIViewport> MainViewport = MakeShareble(new UIViewport());
				{
					MainViewport->SetName("UIViewport2");
					MainDock->AddChild(MainViewport);
				}
			});

		Menu->AddChild(AddNewWindowItem);
	}


	RootWindow->AddWidget(MainDock);
	RootWindow->AddWidget(MainMenuBar);
	//RootWindow->AddWidget(Test);





}



int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	//setlocale(LC_ALL, ".ACP");

	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	CommandConsole::Initialize("Dark Engine Console");
	RHIInit();

	DE_LOG(Launch, Display, TEXT("RHI init"));


	Engine = new DEditorEngine();

	DE_LOG(Launch, Display, TEXT("Create Engine"));
	

	TSharedPtr<UIApplication> CurrentApplication = UIApplication::Create();
	DE_LOG(Launch, Display, TEXT("Create UIApplication"));
	TSharedPtr<UIWindow> RootWindow = MakeShareble(new UIWindow());
	DE_LOG(Launch, Display, TEXT("Create UIWindow"));
	TSharedPtr<FUIRHIRenderer> UIRenderer = MakeShareble(new FUIRHIRenderer());
	DE_LOG(Launch, Display, TEXT("Create UIRenderer"));

	CurrentApplication->InitializeRenderer(UIRenderer);
	DE_LOG(Launch, Display, TEXT("Init UIRenderer"));
	RootWindow->SetbWindowBorder(false);
	CurrentApplication->AddWindow(RootWindow);
	DE_LOG(Launch, Display, TEXT("Add window"));

	RootWindow->ShowWindow();
	DE_LOG(Launch, Display, TEXT("Show window"));
	RootWindow->SetTitle("Edtior");
	DE_LOG(Launch, Display, TEXT("Set title \"%s\""), *RootWindow->GetTitle());


	SetupEditorLayout(RootWindow.get());
	DE_LOG(Launch, Display, TEXT("Setup EditorLayout"));


	
	return 0;

} 

int32 FEngineLoop::Init()
{
	RHIPostInit();
	DE_LOG(Launch, Display, TEXT("RHI Post init"));



	return 0;
}

void FEngineLoop::Tick()
{
	Engine->Tick(1);

	UIApplication::Get()->Tick();





}