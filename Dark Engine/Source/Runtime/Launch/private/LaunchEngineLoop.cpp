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



DECLARE_LOG_CATEGORY(Launch, Display);





struct FEditorLayout
{
	void SetupEditorLayout(UIWindow* InRootWindow)
	{
		RootWindow = InRootWindow;

		MainDock = MakeShareble(new UIDock());
		RootWindow->AddWidget(MainDock);


		TSharedPtr<UILogs> Logs = MakeShareble(new UILogs());
		MainDock->AddChild(Logs);
		Logs->SetName(TEXT("Logs"));
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
		

		TSharedPtr<UIMenuItem> CreateSettings = MakeShareble(new UIMenuItem());
		CreateSettings->SetName(TEXT("Settings"));
		CreateSettings->MenuItemDelegate.Bind([&](bool bPressed)
			{
				if (bPressed)
				{
					MainDock->AddChild(EditorSettings);
				}
				else
				{
					MainDock->RemoveChild(EditorSettings);
				}
			});

		TSharedPtr<UIMenu> File = MakeShareble(new UIMenu());
		MainMenuBar->AddMenu(File);
		File->SetName(TEXT("File"));
		File->AddChild(CreateSettings);
		File->AddChild(ExitButton);
		



		//RootWindow->AddWidget(MainDock);


		//MainDock->AddChild(EditorSettings);
		//RootWindow->AddWidget(EditorSettings);

;
	}

private:
	UIWindow* RootWindow = nullptr;
	TSharedPtr<UIEditorViewport> RootViewport = nullptr;
	TSharedPtr<UIDock> MainDock = nullptr;
	TSharedPtr<UIEditorSettings> EditorSettings;
	TSharedPtr<UIMainMenuBar> MainMenuBar;

} EditorLayout;







int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	//setlocale(LC_ALL, ".ACP");

	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	CommandConsole::Initialize("Dark Engine Console");
	FMath::RandInit(time(0));
	RHIInit();

	DE_LOG(Launch, Log, TEXT("RHI init"));


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
	DE_LOG(Launch, Log, TEXT("Set title \"%s\""), *RootWindow->GetTitle());


	EditorLayout.SetupEditorLayout(RootWindow.get());
	DE_LOG(Launch, Log, TEXT("Setup EditorLayout"));

	
	return 0;

} 

int32 FEngineLoop::Init()
{
	RHIPostInit();
	DE_LOG(Launch, Log, TEXT("RHI Post init"));



	return 0;
}

void FEngineLoop::Tick()
{
	Engine->Tick(1);

	UIApplication::Get()->Tick();
}

void FEngineLoop::Exit()
{
	Logger::Exit();

}