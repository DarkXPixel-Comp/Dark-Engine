#pragma once
#include "CoreTypes.h"
#include "Containers/DarkString.h"

struct FGenericPlatformMisc
{
	static CORE_API void PlatformPreInit();
	static CORE_API void PlatformInit() {}

	static CORE_API const TCHAR* EngineDir();

	static CORE_API const TCHAR* LaunchDir();



};