#include "Misc/Paths.h"
#include "HAL/PlatformMisc.h"

CORE_API FString FPaths::LaunchDir()
{
	return EngineDir() + TEXT("bin/");
}

CORE_API FString FPaths::EngineDir()
{
	FString DefaultEngineDir = RootDir() + TEXT("Dark Engine/");
	return DefaultEngineDir;
}

CORE_API FString FPaths::CacheDir()
{
	return EngineDir() + TEXT("Cache/");
}

CORE_API FString FPaths::EngineBinariesDir()
{
	return EngineDir() + TEXT("bin/");
}

CORE_API FString FPaths::EngineScriptsDir()
{
	return EngineDir() + TEXT("Scripts");
}

CORE_API FString FPaths::EngineContentDir()
{
	return EngineDir() + TEXT("Content/");
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
	return CORE_API EngineDir() + TEXT("Source/");
}

CORE_API FString FPaths::EngineLogDir()
{
	return EngineDir() + TEXT("Logs");
}

CORE_API FString FPaths::EngineShaderDir()
{
	return EngineDir() + TEXT("Shaders/");
}