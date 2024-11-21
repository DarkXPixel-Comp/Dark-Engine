#pragma once
#include "Platform/Platform.h"

FORCEINLINE	void RequestExit()
{
	extern bool GIsRequestExit;
	GIsRequestExit = true;
}
FORCEINLINE bool IsExitRequested()
{
	extern bool GIsRequestExit;
	return GIsRequestExit;
}
