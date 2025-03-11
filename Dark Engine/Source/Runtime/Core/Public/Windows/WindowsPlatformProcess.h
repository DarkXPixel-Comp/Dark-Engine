#pragma once
#include "Containers/DarkString.h"
#include "Windows/WindowsIncludes.h"
#include "WindowsGlobals.h"
#include <filesystem>


static TArray<FString> DllDirectoryStack;
static TArray<FString> DllDirectories;

struct FWindowsPlatformProcess
{
	static void* LoadLibraryWithSearchPaths(const FString& FileName, const TArray<FString>& SearchPaths)
	{
		std::filesystem::path Path = *FileName;

		if (std::filesystem::directory_entry(*FileName).exists())
		{
			void* Handle = nullptr;
			if (Handle = GetModuleHandle(*FileName));
			{
				return Handle;
			}

		}


	}

	static void* GetDllExport(void* DllHandle, const FString& ProcName)
	{
		return ::GetProcAddress((HMODULE)DllHandle, ProcName.ToString().c_str());
	}

	static void* GetDllHandle(const TCHAR* FileName)
	{
		TArray<FString> SearchPaths;
		SearchPaths.Reserve(1 + ((DllDirectoryStack.Num() > 0) ? 1 : 0) + DllDirectories.Num());
		SearchPaths.Add(GetModulesDirectory());
		if (DllDirectoryStack.Num() > 0)
		{
			SearchPaths.Add(DllDirectoryStack.First());
		}
		for (const auto& i : DllDirectories)
		{
			SearchPaths.Add(i);
		}
												  
		return LoadLibraryWithSearchPaths(FileName, SearchPaths);
	}


	static const TCHAR* BaseDir()
	{
		static TCHAR Result[512] = TEXT("");
		if (!Result[0])
		{
			HMODULE hCurrentModule;
			if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&BaseDir, &hCurrentModule) == 0)
			{
				hCurrentModule = hInstance;
			}
			
			GetModuleFileNameW(hCurrentModule, Result, 512);
			FString TempResult = FString(Result).Replace(TEXT('\\'), TEXT('/'));
			wcscpy(Result, *TempResult);

			int32 StringLen = wcslen(Result);
			if (StringLen)
			{
				--StringLen;
				for (; StringLen > 0; --StringLen)
				{
					if (Result[StringLen - 1] == TEXT('/') || Result[StringLen - 1] == TEXT('//'))
					{
						break;
					}
				}

				Result[StringLen] = TEXT('\0');
			}
		}
		return Result;
	}


	static FString GetModulesDirectory()
	{
		static TCHAR Result[512];
		if (Result[0] == 0)
		{
			HMODULE hCurrentModule;
			if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&GetModulesDirectory, &hCurrentModule) == 0)
			{
				hCurrentModule = hInstance;
			}

			GetModuleFileNameW(hCurrentModule, Result, 512);

			*wcsrchr(Result, TEXT('\\')) = TEXT('\0');

		}
		return Result;
	}
};

typedef FWindowsPlatformProcess FPlatformProcess;