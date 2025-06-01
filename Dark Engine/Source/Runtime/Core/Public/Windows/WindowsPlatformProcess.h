#pragma once
#include "Containers/DarkString.h"
#include "Containers/Array.h"
#include "Windows/WindowsIncludes.h"
#include "WindowsGlobals.h"
#include <filesystem>
#include <set>


static TArray<FString> DllDirectoryStack;
static TArray<FString> DllDirectories;

struct FWindowsPlatformProcess
{
	CORE_API static void* LoadLibraryWithSearchPaths(const FString& FileName, const TArray<FString>& SearchPaths)
	{
		std::filesystem::path Path = *FileName;

		if (std::filesystem::directory_entry(*FileName).exists())
		{
			HMODULE Handle = GetModuleHandleW(*FileName);
			if (Handle)
			{
				return Handle;
			}

			TArray<FString> ImportFileNames;
			std::set<FString> VisitedImportNames;
			ResolveMissingImportsRecursive(*FileName, SearchPaths, ImportFileNames, VisitedImportNames);

			for (int32 i = 0; i < ImportFileNames.Num(); ++i)
			{
				const FString ImportFileName = ImportFileNames[i];
				if (!GetModuleHandleW(*ImportFileName))
				{
					return LoadLibraryW(*ImportFileName);
				}
			}
		}
		FString FullFileName = FileName;
		return LoadLibraryW(*FullFileName);
	}

	CORE_API static void ResolveMissingImportsRecursive(const FString& FileName, const TArray<FString>& SearchPaths, TArray<FString> ImportFileNames, std::set<FString>& VisitedImportNames);

	CORE_API static bool ReadLibraryImports(const FString& FileName, TArray<FString>& ImportNames);

	CORE_API static bool ResolveImport(const FString& Name, const TArray<FString>& SearchPaths, FString& OutFileName);

	CORE_API static bool ReadLibrartImportsFromMemory(const IMAGE_DOS_HEADER* Header, TArray<FString>& ImportNames);

	CORE_API static void FreeDllHandle(void* DllHandle)
	{
		FreeLibrary((HMODULE)DllHandle);
	}

	CORE_API static void* GetDllExport(void* DllHandle, const FString& ProcName)
	{
		return ::GetProcAddress((HMODULE)DllHandle, ProcName.ToString().c_str());
	}

	CORE_API static void* GetDllHandle(const TCHAR* FileName)
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

	CORE_API static void SetCurrentWorkingDirectoryToBaseDir();

	CORE_API static FString GetCurrentWorkingDirectory();


	CORE_API static const TCHAR* BaseDir()
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
					if (Result[StringLen - 1] == TEXT('/') || Result[StringLen - 1] == TEXT('\\'))
					{
						break;
					}
				}

				Result[StringLen] = TEXT('\0');
			}

			auto path = std::filesystem::canonical(Result);
			wcscpy(Result, path.generic_wstring().c_str());
		}
		return Result;
	}


	CORE_API static FString GetModulesDirectory()
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