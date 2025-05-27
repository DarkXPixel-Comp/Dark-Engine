#pragma once
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Windows/WindowsGlobals.h"
#include <filesystem>

struct FWindowsPlatformMisc : public FGenericPlatformMisc
{
	static CORE_API FString EngineDir(bool bIsBackSlash = false)
	{
		FString Result = std::filesystem::path(LaunchDir(bIsBackSlash) / TEXT("../../../")).wstring();

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
		Path = std::filesystem::path(Path).parent_path().wstring();
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
		return ::MessageBox(NULL, Text, Caption, MB_YESNO | MB_ICONERROR | MB_SYSTEMMODAL) == IDYES;
	}

	static CORE_API void DebugPrint(const FString& Text);

	static CORE_API bool IsDebuggerPresent();

	static CORE_API void PromptForRemoteDebugging();

	static CORE_API void OpenHTML(const FString& Path);

	static CORE_API void GetStackTrace(FString& Out);

	static CORE_API void GetStackTrace(CONTEXT* context, FString& Out);

	static CORE_API FString GetExceptionCodeString(DWORD code);
};

typedef FWindowsPlatformMisc FPlatformMisc;