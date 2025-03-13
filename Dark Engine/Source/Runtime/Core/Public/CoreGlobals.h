#pragma once
#include "Platform/Platform.h"

FORCEINLINE	void CORE_API RequestExit()
{
	CORE_API extern bool GIsRequestExit;
	GIsRequestExit = true;
}
FORCEINLINE bool CORE_API IsExitRequested()
{
	CORE_API extern bool GIsRequestExit;
	return GIsRequestExit;
}
