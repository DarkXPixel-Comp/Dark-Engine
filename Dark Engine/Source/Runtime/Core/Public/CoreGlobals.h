#pragma once


#include "Core.h"
#include "CoreTypes.h"
#include "CorePlatform.h"


extern DENGINE_API bool GIsRequestingExit;

FORCEINLINE bool IsExitRequested()
{
	return GIsRequestingExit;
}