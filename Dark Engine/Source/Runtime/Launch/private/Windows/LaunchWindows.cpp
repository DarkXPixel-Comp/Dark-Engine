
#include <Settings/PerEngineSettings.h>
//#include <Launch/Resource/resource.h>
#include <Application/Application.h>
#include <DEngine.h>
#include <Logging/Logger.hpp>
#include <Windows/WindowsSystemIncludes.h>







extern int32 GuardedMain(const TCHAR* CmdLine);

void InvalidParameterHandler(const TCHAR* Expr,
							 const TCHAR* Func,
							 const TCHAR* File,
							 uint32 Line,
							 uintptr_t Reserved)
{
	throw;

}




void SetupWindowsEnviroment()
{
	_set_invalid_parameter_handler(InvalidParameterHandler);
}



int32 LaunchWindowsStartup(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char* pCmdLine, int32 nCmdShow, TCHAR* CmdLine)
{
	SetupWindowsEnviroment();

	int32_t ErrorCode = 0;
	hInstance = hInInstance;

	Application::Instance()->Initialize(hInInstance, hPrevInstance, pCmdLine, nCmdShow);

	ErrorCode = GuardedMain(CmdLine);
	

	ErrorCode = GEngine.Initialize();
	ErrorCode = GEngine.PostInit();


	while (GEngine.isAppWork())
	{
		GEngine.UpdateLoop();
	}




	GEngine.Shutdown();


	Logger::wait();
	Logger::Exit();


	return ErrorCode;


}




int32_t WINAPI WinMain(
	_In_ HINSTANCE hInInstance,
	_In_opt_ HINSTANCE prevInstance,
	_In_ char* lpCmdLine,
	_In_ int32_t nCmdShow
)
{
	int32 Result = LaunchWindowsStartup(hInInstance, prevInstance, lpCmdLine, nCmdShow, nullptr);
	
	return Result;
}


int32_t DENGINE_API WWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, char* lpCmdLine, int32_t nCmdShow)
{
	return WinMain(hInstance, prevInstance, lpCmdLine, nCmdShow);
}