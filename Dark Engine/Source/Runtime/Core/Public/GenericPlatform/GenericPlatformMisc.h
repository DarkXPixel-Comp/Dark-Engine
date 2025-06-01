#pragma once

#include "Platform/Platform.h"
#include "Containers/DarkString.h"

struct FGenericPlatformMisc
{
	static CORE_API void PlatformPreInit() {}
	static CORE_API void PlatformInit() {}

	static CORE_API FString EngineDir();

	static CORE_API FString LaunchDir() { return FString(); }

	static CORE_API FString GetPathModule() { return FString(); }

	static CORE_API bool CreateMessageBoxError(const TCHAR* Text, const TCHAR* Caption) { return false; }

	static CORE_API void LocalPrint(const TCHAR* Str);

	static CORE_API void CacheLaunchDir();

	static CORE_API const TCHAR* RootDir();
};