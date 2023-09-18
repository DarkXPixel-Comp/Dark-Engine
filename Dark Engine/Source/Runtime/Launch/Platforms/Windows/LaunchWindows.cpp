#include <Core/Settings/PerEngineSettings.h>
#include <Launch/Resource/resource.h>
#include <Core/Application/Application.h>
#include <Engine/public/DEngine.h>
#include <Core/Logging/Logger.hpp>

#include "LaunchWindows.h"


void WindowsEnviromentSetup()
{
	//ParseCommandLineArgumets();
	PerEngineSettings::SetProjectName(IDS_PROJECT_NAME);
	PerEngineSettings::SetShortName(IDS_SHORT_NAME);

	PerEngineSettings::SetMainIcon(5);

}








int32_t WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE prevInstance,
	_In_ char* lpCmdLine,
	_In_ int32_t nCmdShow
)
{
	int32_t ErrorCode = 0;

	Application::Instance()->Initialize(hInstance, prevInstance, lpCmdLine, nCmdShow);
	WindowsEnviromentSetup();

	GEngine.Initialize();
	GEngine.PostInit();


	while (GEngine.isAppWork())
	{
		GEngine.UpdateLoop();
	}



	GEngine.Shutdown();




	Logger::wait();


	return ErrorCode;
}


int32_t DENGINE_API WWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, char* lpCmdLine, int32_t nCmdShow)
{
	return WinMain(hInstance, prevInstance, lpCmdLine, nCmdShow);
}