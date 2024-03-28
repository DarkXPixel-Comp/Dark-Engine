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


		MainMenuBar = MakeShareble(new UIMainMenuBar());
		MainDock->SetMainMenuBar(MainMenuBar);

		EditorSettings = MakeShareble(new UIEditorSettings());
		EditorSettings->SetEditorViewport(RootViewport);

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
		


		RootViewport = MakeShareble(new UIEditorViewport());
		{
			RootViewport->SetName(TEXT("EditorViewport"));
			MainDock->AddChild(RootViewport);
		}


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
	TSharedPtr<UIMainMenuBar> MainMenuBar = MakeShareble(new UIMainMenuBar());

} EditorLayout;







int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	//setlocale(LC_ALL, ".ACP");

	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	CommandConsole::Initialize("Dark Engine Console");
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
	static int32 Counter = 0;

	if (Counter++ == 100)
	{
		//GIsRequestingExit = true;
	}

	Engine->Tick(1);

	UIApplication::Get()->Tick();





}