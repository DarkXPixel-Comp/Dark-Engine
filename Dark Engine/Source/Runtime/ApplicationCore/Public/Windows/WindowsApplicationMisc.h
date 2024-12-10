#pragma once

#include "GenericPlatform/GenericApplicationMisc.h"
#include "GenericPlatform/GenericApplication.h"

struct FWindowsApplicationMisc : public FGenericPlatformApplicationMisc
{
	static FGenericApplication* CreateApplication();
};

typedef FWindowsApplicationMisc FPlatformApplicationMisc;