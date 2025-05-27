#pragma once
#include <Windows.h>

class FWindowsLayout
{
public:
	RECT GetBorderThckness() const;
};


typedef FWindowsLayout FPlatformLayout;