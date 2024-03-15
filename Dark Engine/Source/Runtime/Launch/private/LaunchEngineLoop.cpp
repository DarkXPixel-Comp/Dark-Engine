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



DECLARE_LOG_CATEGORY(TEST, All);

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


	DE_LOG(TEST, Warning, TEXT("TEST"));

	//TSharedPtr<UIButton> ExitButton = MakeShareble(new UIButton());
	//{
	//	ExitButton->SetPostion(FIntPoint(0, 0));
	//	ExitButton->SetSize(FIntPoint(80, 80));
	//	ExitButton->ButtonDelegate.Bind([&]()
	//		{
	//			GIsRequestingExit = true;
	//		});
	//	MainMenuBar->AddChild(ExitButton);
	//}

	//ExitButton->SetName(TEXT("Exit"));

	TSharedPtr<UIViewport> MainViewport = MakeShareble(new UIViewport());
	{
		MainViewport->SetName("UIViewport");
		MainDock->AddChild(MainViewport);
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
	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	CommandConsole::Initialize("Dark Engine Console");
	RHIInit();


	Engine = new DEditorEngine();
	

	TSharedPtr<UIApplication> CurrentApplication = UIApplication::Create();
	TSharedPtr<UIWindow> RootWindow = MakeShareble(new UIWindow());
	TSharedPtr<FUIRHIRenderer> UIRenderer = MakeShareble(new FUIRHIRenderer());

	CurrentApplication->InitializeRenderer(UIRenderer);
	RootWindow->SetbWindowBorder(false);
	CurrentApplication->AddWindow(RootWindow);

	RootWindow->ShowWindow();
	RootWindow->SetTitle("Edtior");


	SetupEditorLayout(RootWindow.get());


	
	return 0;

} 

int32 FEngineLoop::Init()
{
	RHIPostInit();



	return 0;
}

void FEngineLoop::Tick()
{
	UIApplication::Get()->Tick();
	//Engine->Tick(1);





}