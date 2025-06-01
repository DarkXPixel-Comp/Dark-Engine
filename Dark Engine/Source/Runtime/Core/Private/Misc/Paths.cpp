#include "Misc/Paths.h"
#include "Platform/PlatformMisc.h"


FString FPaths::LaunchDir()
{
	return FPlatformMisc::LaunchDir();
}

FString FPaths::EngineDir()
{
	return FPlatformMisc::EngineDir();
}

FString FPaths::CacheDir()
{
	return EngineDir() + TEXT("Cache/");
}

FString FPaths::EngineBinariesDir()
{
	return EngineDir() + TEXT("Binaires/");
}

FString FPaths::EngineScriptsDir()
{
	return EngineDir() + TEXT("Scripts");
}

FString FPaths::EngineContentDir()
{
	return EngineDir() + TEXT("Content/");
}
FString FPaths::EngineConfigDir()
{
	return EngineDir() + TEXT("Config/");
}

FString FPaths::RootDir()
{
	return FPlatformMisc::RootDir();
}

FString FPaths::CombineDir(FString Str1, FString Str2)
{
	return Str1 + Str2;
}


FString FPaths::SourceDir()
{
	return EngineDir() + TEXT("Source/");
}

FString FPaths::EngineLogDir()
{
	return EngineDir() + TEXT("Logs/");
}

FString FPaths::EngineShaderDir()
{
	return EngineDir() + TEXT("Shaders/");
}

bool FPaths::FileExists(const FString& Path)
{
	return FPlatformMisc::FileExists(Path);
}

FString FPaths::ConvertRelativeToFullPath(const FString& InFileName)
{
	FString FileName = std::filesystem::absolute(*InFileName).generic_wstring();

	FPlatformMisc::NormalizeWindowsPath(FileName);
	return FileName;
}