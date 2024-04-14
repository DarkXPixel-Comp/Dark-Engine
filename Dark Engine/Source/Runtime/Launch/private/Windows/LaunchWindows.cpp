
#include <Settings/PerEngineSettings.h>
//#include <Launch/Resource/resource.h>
#include <Application/Application.h>
#include <DEngine.h>
#include <Logging/Logger.h>
#include <Windows/WindowsSystemIncludes.h>
#include "Logging/LogMacros.h"




DECLARE_LOG_CATEGORY(LaunchWindowsLog, Display);


extern int32 GuardedMain(const TCHAR* CmdLine);

void InvalidParameterHandler(const TCHAR* Expr,
							 const TCHAR* Func,
							 const TCHAR* File,
							 uint32 Line,
							 uintptr_t Reserved)
{
	DE_LOG(LaunchWindowsLog, Fatal, TEXT("%s, Func = (%s) In %s(%i)"), Expr, Func, File, Line);
}




static void SetupWindowsEnviroment()
{
	_set_invalid_parameter_handler(InvalidParameterHandler);
}



int32 LaunchWindowsStartup(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char* pCmdLine, int32 nCmdShow, TCHAR* CmdLine)
{
	SetupWindowsEnviroment();

	//setlocale(LC_ALL, ".utf8");

	int32_t ErrorCode = 0;
	hInstance = hInInstance;


	ErrorCode = GuardedMain(CmdLine);
	


	return ErrorCode;


}




int32_t WINAPI WinMain(
	_In_ HINSTANCE hInInstance,
	_In_opt_ HINSTANCE prevInstance,
	_In_ char* lpCmdLine,
	_In_ int32_t nCmdShow
)
{
	int32 Result = LaunchWindowsStartup(hInInstance, prevInstance, lpCmdLine, nCmdShow, *FString(lpCmdLine));
	
	return Result;
}


int32_t DENGINE_API WWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, char* lpCmdLine, int32_t nCmdShow)
{
	return WinMain(hInstance, prevInstance, lpCmdLine, nCmdShow);
}