#pragma once
#include "Platform/Platform.h"

CORE_API extern bool GIsRequestExit;

FORCEINLINE	void CORE_API RequestExit()
{
	GIsRequestExit = true;
}
FORCEINLINE bool CORE_API IsExitRequested()
{
	return GIsRequestExit;
}
