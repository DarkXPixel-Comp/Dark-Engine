#pragma once


#include "CoreTypes.h"
#include "HAL/PlatformMemory.h"
#include <Microsoft/MinimalWindowsApi.h>
#include "GenericPlatform/GenericPlatformMisc.h"

#undef GetEnvironmentVariable
#undef SetEnvironmentVariable


struct FWindowsPlatformMisc : public FGenericPlatformMisc
{
	static CORE_API void PlatformPreInit();

	static CORE_API void PlatformInit();

	static CORE_API FString GetEnvironmentVariable(const TCHAR* VariableName);

	static CORE_API void SetEnvironmentVariable(const TCHAR* VariableName, const TCHAR* Value);






};



typedef FWindowsPlatformMisc;



