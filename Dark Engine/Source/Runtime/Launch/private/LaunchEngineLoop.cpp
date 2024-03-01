#include "LaunchEngineLoop.h"
#include "RHI.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Application/UIApplication.h"
#include "Engine/EditorEngine.h"
#include "Rendering/UIRHIRenderer.h"


int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	CommandConsole::Initialize("Dark Engine Console");
	RHIInit();


	Engine = new DEditorEngine();
	

	TSharedPtr<UIApplication> CurrentApplication = UIApplication::Create();

	TSharedPtr<FUIRHIRenderer> UIRenderer = MakeShareble(new FUIRHIRenderer());

	CurrentApplication->InitializeRenderer(UIRenderer);

	TSharedPtr<UIWindow> RootWindow = MakeShareble(new UIWindow());
	RootWindow->SetbWindowBorder(false);
	CurrentApplication->AddWindow(RootWindow);
	RootWindow->ShowWindow();
	TSharedPtr<UIViewport> GameViewport = MakeShareble(new UIViewport());
	//GameViewport->SetViewport()
	RootWindow->SetTitle("Edtior");
	GameViewport->SetPostion({ 0, 0 });
	GameViewport->SetSize({ 800, 600 });
	//RootWindow->Maximize();
	RootWindow->AddWidget(GameViewport);

	





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