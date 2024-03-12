#pragma once
#include "CoreTypes.h"
#include "Containers/DarkString.h"

struct FGenericPlatformMisc
{
	static CORE_API void PlatformPreInit();
	static CORE_API void PlatformInit() {}

	static CORE_API FString EngineDir() { return FString(); }

	static CORE_API FString LaunchDir() { return FString(); }

	static CORE_API FString GetPathModule() { return FString(); }



};