#include "LaunchEngineLoop.h"
#include "RHI.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Application/UIApplication.h"


int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR);
	CommandConsole::Initialize("Dark Engine Console");
	RHIInit();

	TSharedPtr<UIApplication> CurrentApplication = UIApplication::Create();
	//CurrentApplication->InitializeRenderer(nullptr);

	TSharedPtr<UIWindow> RootWindow = MakeShareble(new UIWindow());

	CurrentApplication->AddWindow(RootWindow);

	





	return 0;

} 

int32 FEngineLoop::Init()
{



	return 0;
}