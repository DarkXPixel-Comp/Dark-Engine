#pragma once

#include <Source/Runtime/Core/CoreMinimal.h>


namespace primal::platform
{
	class window
	{
		void set_fullscreen(bool is_fullscreen);
		bool is_fullscreen();
		void* handle();
		void set_caption(const char* caption);
		void resize(uint32_t width, uint32_t height);
		bool is_closed();


	};

}
