#pragma once


#include "WindowsApplication.h"
#include "GenericPlatform/GenericApplicationMisc.h"
#include "CoreMinimal.h"


struct FWindowsApplicationMisc : public FGenericPlatformApplicationMisc
{
	static FGenericApplication* CreateApplication();
};


typedef FWindowsApplicationMisc FPlatformApplicationMisc;


