#pragma once
#include "Platform.h"

namespace Windows
{
	HINSTANCE hInstance;
	HWND hWnd;
	RECT fullscreen_area;
	MSG msg;

	//const uint32_t bufferCount = 3;

	uint32_t g_ClientWidth;
	uint32_t g_ClientHeight;
	int32_t nCmdShow;

	bool g_UseWarp;
	bool AppQuit;

	bool isFullScreen = false;

	void (*HandlerMessages)();

}

#ifdef _WIN64
namespace primal::platform
{
	//window create_window(const window_init_info* const init_info)
	//{
	//	window_proc callback	{ init_info ? init_info->callback : nullptr };
	//	window_handle parent	{ init_info ? init_info->parent : nullptr };

	//	// Setup a window class


	//	// Register the window class



	//	// Create an instance of the window class









	//}


}

#endif