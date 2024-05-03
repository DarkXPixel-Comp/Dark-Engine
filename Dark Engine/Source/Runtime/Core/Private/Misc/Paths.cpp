#include "Misc/Paths.h"
#include "HAL/PlatformMisc.h"

CORE_API FString FPaths::LaunchDir()
{
	return EngineDir() + "bin/";
}

CORE_API FString FPaths::EngineDir()
{
	FString DefaultEngineDir = RootDir() + "Dark Engine/";
	return DefaultEngineDir;
}

CORE_API FString FPaths::CacheDir()
{
	return EngineDir() + TEXT("Cache/");
}

CORE_API FString FPaths::EngineBinariesDir()
{
	return EngineDir() + "bin/";
}

CORE_API FString FPaths::EngineContentDir()
{
	return EngineDir() + "Content/";
}
CORE_API FString FPaths::ConfigDir()
{
	return EngineDir() + TEXT("Config");
}

CORE_API FString FPaths::RootDir()
{
	return FPlatformMisc::EngineDir();
}

CORE_API FString FPaths::CombineDir(FString Str1, FString Str2)
{
	return Str1 + Str2;
}


CORE_API FString FPaths::SourceDir()
{
	return CORE_API EngineDir() + "Source/";
}

CORE_API FString FPaths::EngineShaderDir()
{
	return SourceDir() + "Shaders/";
}