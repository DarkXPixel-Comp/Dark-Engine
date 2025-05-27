#pragma once
#include "Platform/Platform.h"



class FWindowsConsoleOutputDevice
{
private:
	uint16 TextAttribute;
	class FConsoleWindow;
	FConsoleWindow* Window;

public:
	APPLICATIONCORE_API void Show(bool bShowWindow);


};