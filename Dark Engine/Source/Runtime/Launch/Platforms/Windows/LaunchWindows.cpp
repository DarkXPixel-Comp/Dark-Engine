#include <Runtime/Core/Core.h>
#include "../../Resource/resource.h"
#include "../../../Core/Threads/Threads.h"

#include <iostream>

#include <TlHelp32.h>



LRESULT CALLBACK WndProc
(
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);



void ParseCommandLineArgumets()
{
	int argc;

	wchar_t** argv = CommandLineToArgvW(GetCommandLine(), &argc);

	for (size_t i = 0; i < argc; i++)
	{
		if (wcscmp(argv[i], L"-w") == 0 || wcscmp(argv[i], L"-width") == 0)
		{
			Windows::g_ClientWidth = wcstol(argv[++i], nullptr, 10);
		}

		if (wcscmp(argv[i], L"-h") == 0 || wcscmp(argv[i], L"-height") == 0)
		{
			Windows::g_ClientHeight = wcstol(argv[++i], nullptr, 10);
		}

		if (wcscmp(argv[i], L"-warp") == 0)
		{
			Windows::g_UseWarp = true;
		}

	}

	LocalFree(argv);	

}

void HandlerMessages()
{
	GetMessageW(&Windows::msg, Windows::hWnd, 0, 0);
	{
		TranslateMessage(&Windows::msg);
		DispatchMessageW(&Windows::msg);
	}

}


void WindowsEnviromentSetup()
{
	//ParseCommandLineArgumets();
	PerEngineSettings::SetProjectName(IDS_PROJECT_NAME);
	PerEngineSettings::SetShortName(IDS_SHORT_NAME);

	PerEngineSettings::SetMainIcon(5);

}





void f(int t)
{
	MessageBox(0, std::wstring(L"Test" + std::to_wstring(t)).c_str(), L"TEST", 0);
}


int32_t WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE prevInstance,
	_In_ char* lpCmdLine,
	_In_ int32_t nCmdShow
)
{
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	int32_t ErrorCode = 0;

	Application::Instance()->Initialize(hInstance, prevInstance, lpCmdLine, nCmdShow);
	WindowsEnviromentSetup();



	DEngineInitInfo info = {};

	info.WindowInitInfo.height = 1080;
	info.WindowInitInfo.width = 1920;
	info.nameGame = "Test";
	info.WindowInitInfo.leftX = 0;
	info.WindowInitInfo.topY = 0;
	info.platformCmdShow = nCmdShow;
	

	try
	{
		GEngine.Initialize(info);
	}
	catch (const std::exception& ex)
	{
		GEngine.Quit();
	}
	catch (...)
	{
		GEngine.Quit();
	}





	ErrorCode = guardedMain();


	

	Logger::wait();


	return ErrorCode;
}