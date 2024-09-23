#pragma once


#include "Core.h"
#include "CoreTypes.h"
#include "CorePlatform.h"


extern bool GIsRequestingExit;
extern bool GIsTitleBarHovered;
extern class FConfigCache* GConfig;
extern int32 GRenderMode; //<-TEST
extern float GMaxFPS;
extern float GNearClippingPlane;
extern bool GFXAAEnable;
extern int32 GCurrentEntity;
extern FString GAdditionalErrorMsg;

FORCEINLINE bool IsExitRequested()
{
	return GIsRequestingExit;
}

FORCEINLINE void RequestExit()
{
	GIsRequestingExit = true;
}

FORCEINLINE FString GetAdditionalErrorMsg()
{
	FString ErrorMsg = std::move(GAdditionalErrorMsg);
	GAdditionalErrorMsg = TEXT("");
	return ErrorMsg;
}