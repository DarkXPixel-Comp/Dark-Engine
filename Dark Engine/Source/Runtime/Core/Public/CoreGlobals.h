#pragma once


#include "Core.h"
#include "CoreTypes.h"
#include "CorePlatform.h"


extern DENGINE_API bool GIsRequestingExit;
extern bool GIsTitleBarHovered;

FORCEINLINE bool IsExitRequested()
{
	return GIsRequestingExit;
}