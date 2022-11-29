#pragma once

#include <Source/Runtime/Core/CoreMinimal.h>

#ifdef _WIN64



namespace primal::platform
{
	using window_handle = HWND;
	using window_proc = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);
	 
	struct window_init_info
	{
		window_proc		callback{ nullptr };
		window_handle	parent{ nullptr };
		const char*		caption{ nullptr };
		uint32_t		left{ 0 };
		uint32_t		top{ 0 };
		uint32_t		width{ 800 };
		uint32_t		height{ 600 };


	};

}

#endif