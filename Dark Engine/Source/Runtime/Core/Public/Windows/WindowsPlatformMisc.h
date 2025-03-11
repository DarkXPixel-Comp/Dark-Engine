#pragma once
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Windows/WindowsGlobals.h"
#include <filesystem>

struct FWindowsPlatformMisc : public FGenericPlatformMisc
{
	static CORE_API FString EngineDir(bool bIsBackSlash = false)
	{
		TCHAR Result[256];
		PathCchCombine(Result, 256, *LaunchDir(true), TEXT("..\\..\\"));
		if (!bIsBackSlash)
		{
			std::replace(std::begin(Result), std::end(Result), TEXT('\\'), TEXT('/'));
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

	static void NormalizeWindowsPath(FString& Path)
	{
		std::filesystem::absolute(*Path);



		for (auto& Char : Path)
		{
			if (Char == TEXT('/'))
			{
				Char = TEXT('\\');
			}
		}
	}

	static CORE_API bool FileExists(const FString& InPath)
	{
		FString Path = InPath;
		NormalizeWindowsPath(Path);
		DWORD Result = GetFileAttributesW(*Path);
		if (Result != 0xFFFFFFFF && !(Result & FILE_ATTRIBUTE_DIRECTORY))
		{
			return true;
		}
		return false;
	}

	static CORE_API bool CreateMessageBoxError(const TCHAR* Text, const TCHAR* Caption)
	{
		::MessageBox(NULL, Text, Caption, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return true;
	}
};

typedef FWindowsPlatformMisc FPlatformMisc;