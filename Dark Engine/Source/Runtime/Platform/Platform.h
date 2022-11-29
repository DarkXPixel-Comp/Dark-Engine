#pragma once
#include "../Core/CoreMinimal.h"

namespace Windows 
{
	extern HINSTANCE hInstance;
	extern HWND hWnd;
	extern RECT fullscreen_area;
	extern MSG msg;

//	extern const uint32_t bufferCount;

	constexpr uint32_t bufferCount = 3;

	extern uint32_t g_ClientWidth;
	extern uint32_t g_ClientHeight;
	extern int32_t nCmdShow;

	extern bool g_UseWarp;
	extern bool AppQuit;

	extern bool isFullScreen;

	extern void (*HandlerMessages)();

};


namespace primal::platform
{
	/*struct window_init_info;

	window create_window(const window_init_info* const init_info = nullptr);
	void remove_window();*/

}
