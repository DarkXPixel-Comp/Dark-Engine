#pragma once


#include "Core.h"
#include "CoreTypes.h"
#include "CorePlatform.h"


extern bool GIsRequestingExit;
extern bool GIsTitleBarHovered;
extern class FConfigCache* GConfig;
extern int32 GRenderMode; //<-TEST

FORCEINLINE bool IsExitRequested()
{
	return GIsRequestingExit;
}

FORCEINLINE void RequestExit()
{
	GIsRequestingExit = true;
}