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


int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	CommandConsole::Initialize("Dark Engine Console");
	RHIInit();


	Engine = new DEditorEngine();
	

	TSharedPtr<UIApplication> CurrentApplication = UIApplication::Create();

	TSharedPtr<FUIRHIRenderer> UIRenderer = MakeShareble(new FUIRHIRenderer());
	TSharedPtr<UIMainMenuBar> MenuBar = MakeShareble(new UIMainMenuBar());
	TSharedPtr<UIMenu> Menu = MakeShareble(new UIMenu());
	TSharedPtr<UIButton> Button = MakeShareble(new UIButton());
	TSharedPtr<UINodeEditor> NodeEditor = MakeShareble(new UINodeEditor());
	Button->ButtonDelegate.Bind([&]()
		{
			GIsRequestingExit = true;
		});
	Button->SetName(TEXT("Exit"));
	Button->SetSize(FIntPoint(40, 40));
	MenuBar->SetSize(FIntPoint(0, 40));
	Menu->AddChild(Button);
	Menu->SetName(TEXT("MENU"));
	Menu->SetSize(FIntPoint(40, MenuBar->GetRecommendedHeightObject()));
	MenuBar->AddChild(Menu);

	FString Test = FPaths::EngineContentDir();

	CurrentApplication->InitializeRenderer(UIRenderer);

	TSharedPtr<UIWindow> RootWindow = MakeShareble(new UIWindow());
	RootWindow->SetbWindowBorder(false);
	CurrentApplication->AddWindow(RootWindow);
	RootWindow->ShowWindow();
	TSharedPtr<UIViewport> GameViewport = MakeShareble(new UIViewport());
	//GameViewport->SetViewport()
	RootWindow->SetTitle("Edtior");
	GameViewport->SetPostion({ 20, 20 });
	GameViewport->SetSize({ 800, 600 });
	//RootWindow->Maximize();
	RootWindow->AddWidget(GameViewport);
	RootWindow->AddWidget(MenuBar);
	//RootWindow->AddWidget(NodeEditor);


	





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