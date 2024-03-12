#pragma once


#include "CoreTypes.h"
#include "HAL/PlatformMemory.h"
#include <Microsoft/MinimalWindowsApi.h>
#include "GenericPlatform/GenericPlatformMisc.h"
#include <PathCch.h>

#undef GetEnvironmentVariable
#undef SetEnvironmentVariable


struct FWindowsPlatformMisc : public FGenericPlatformMisc
{
	static CORE_API void PlatformPreInit();

	static CORE_API void PlatformInit();

	static CORE_API FString GetEnvironmentVariable(const TCHAR* VariableName);

	static CORE_API void SetEnvironmentVariable(const TCHAR* VariableName, const TCHAR* Value);

	static CORE_API FString GetPathModule();

	static CORE_API FString EngineDir(bool bIsBackSlash = false)
	{
		FString Result(256);
		PathCchCombine(*Result, 256, *LaunchDir(true), TEXT("..\\..\\"));
		if (!bIsBackSlash)
		{
			Result.Replace('\\', '/');
		}
		return Result;
	}

	static CORE_API FString LaunchDir(bool bIsBackSlash = false)
	{
		FString Path(256);
		GetModuleFileName(NULL, *Path, 256);
		PathCchRemoveFileSpec(*Path, 256);
		if (!bIsBackSlash)
		{
			Path.Replace('\\', '/');
		}
		return Path;
	}






};



typedef FWindowsPlatformMisc FPlatformMisc;



