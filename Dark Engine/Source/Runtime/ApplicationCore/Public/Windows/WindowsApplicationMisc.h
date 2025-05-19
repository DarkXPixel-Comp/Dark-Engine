#pragma once

#include "GenericPlatform/GenericApplicationMisc.h"
#include "GenericPlatform/GenericApplication.h"

struct FWindowsApplicationMisc : public FGenericPlatformApplicationMisc
{
	static APPLICATIONCORE_API FGenericApplication* CreateApplication();
	static APPLICATIONCORE_API float GetDPIScaleFactorAtPoint(float X, float Y);
	static APPLICATIONCORE_API void SetHighDPIMode();
	
};

typedef FWindowsApplicationMisc FPlatformApplicationMisc;