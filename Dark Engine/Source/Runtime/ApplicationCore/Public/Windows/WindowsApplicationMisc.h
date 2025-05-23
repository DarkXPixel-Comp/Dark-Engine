#pragma once

#include "GenericPlatform/GenericApplicationMisc.h"
#include "GenericPlatform/GenericApplication.h"

struct FWindowsApplicationMisc : public FGenericPlatformApplicationMisc
{
	static APPLICATIONCORE_API FGenericApplication* CreateApplication();
	static APPLICATIONCORE_API float GetDPIScaleFactorAtPoint(float X, float Y);
	static APPLICATIONCORE_API void SetHighDPIMode();
	static APPLICATIONCORE_API int32 GetMonitorDPI(const FMonitorInfo& MonitorInfo);
	static APPLICATIONCORE_API void Shutdown();
	
};

typedef FWindowsApplicationMisc FPlatformApplicationMisc;