#include "Windows/WindowsGlobals.h"
#include "Platform/Platform.h"
#include "Containers/DarkString.h"


extern int32 GuardedMain(const TCHAR* CmdLine);


void InvalidParameterHandler(const TCHAR* Expr,
	const TCHAR* Func,
	const TCHAR* File,
	uint32 Line,
	uintptr_t Reserved)
{
	//DE_LOG(LaunchWindowsLog, Fatal, TEXT("%s, Func = (%s) In %s(%i)"), Expr, Func, File, Line);
}

static void SetupWindowsEnviroment()
{
	_set_invalid_parameter_handler(InvalidParameterHandler);
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

int32 LaunchWindowsStartup(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char* pCmdLine, int32 nCmdShow, TCHAR* CmdLine)
{
	SetupWindowsEnviroment();
	int32 ErrorCode = 0;
	hInstance = hInInstance;

	ErrorCode = GuardedMain(CmdLine);



	return ErrorCode;
}


int32 WINAPI WinMain(
	_In_ HINSTANCE hInInstance,
	_In_opt_ HINSTANCE prevInstance,
	_In_ ANSICHAR* lpCmdLine,
	_In_ int32 nCmdShow
)
{
	int32 Result = LaunchWindowsStartup(hInInstance, hInInstance, lpCmdLine, nCmdShow, *FString(lpCmdLine));
	return Result;
}
