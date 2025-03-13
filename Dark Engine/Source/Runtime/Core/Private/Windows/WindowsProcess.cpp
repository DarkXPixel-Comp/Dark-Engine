#include "Windows/WindowsPlatformProcess.h"
#include <Misc/Paths.h>


void FWindowsPlatformProcess::ResolveMissingImportsRecursive(const FString& FileName, const TArray<FString>& SearchPaths, TArray<FString> ImportFileNames, std::set<FString>& VisitedImportNames)
{
	TArray<FString> ImportNames;

	if (ReadLibraryImports(FileName, ImportNames))
	{
		for (int32 i = 0; i < ImportNames.Num(); ++i)
		{
			const FString& ImportName = ImportNames[i];
			if (!VisitedImportNames.contains(ImportName))
			{
				VisitedImportNames.insert(ImportName);

				if (GetModuleHandle(*ImportName) == NULL)
				{
					FString ImportFileName;
					if (ResolveImport(ImportName, SearchPaths, ImportFileName))
					{
						ResolveMissingImportsRecursive(ImportFileName, SearchPaths, ImportFileNames, VisitedImportNames);
						ImportFileNames.Add(ImportFileName);
					}
				}
			}
		}
	}
}


bool FWindowsPlatformProcess::ReadLibraryImports(const FString& FileName, TArray<FString>& ImportNames)
{
	bool Result = false;

	HANDLE NewFileHandle = CreateFileW(*FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (NewFileHandle == INVALID_HANDLE_VALUE)
		return false;

	HANDLE MappingHandle = CreateFileMappingW(NewFileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
	if (MappingHandle != NULL)
	{
		void* pData = MapViewOfFile(MappingHandle, FILE_MAP_READ, 0, 0, 0);
		if (pData != nullptr)
		{
			const IMAGE_DOS_HEADER* Header = (const IMAGE_DOS_HEADER*)pData;
			Result = ReadLibrartImportsFromMemory(Header, ImportNames);
			UnmapViewOfFile(pData);
		}
		CloseHandle(MappingHandle);
	}
	CloseHandle(NewFileHandle);

	return Result;
}

static const void* MapRvaToPointer(const IMAGE_DOS_HEADER* Header, const IMAGE_NT_HEADERS* NtHeader, size_t Rva)
{
	const IMAGE_SECTION_HEADER* SectionHeaders = (const IMAGE_SECTION_HEADER*)(NtHeader + 1);
	for (size_t SectionIdx = 0; SectionIdx < NtHeader->FileHeader.NumberOfSections; SectionIdx++)
	{
		const IMAGE_SECTION_HEADER* SectionHeader = SectionHeaders + SectionIdx;
		if (Rva >= SectionHeader->VirtualAddress && Rva < SectionHeader->VirtualAddress + SectionHeader->SizeOfRawData)
		{
			return (const BYTE*)Header + SectionHeader->PointerToRawData + (Rva - SectionHeader->VirtualAddress);
		}
	}
	return NULL;
}

bool FWindowsPlatformProcess::ReadLibrartImportsFromMemory(const IMAGE_DOS_HEADER* Header, TArray<FString>& ImportNames)
{
	if (Header->e_magic == IMAGE_DOS_SIGNATURE)
	{
		IMAGE_NT_HEADERS* NtHeader = (IMAGE_NT_HEADERS*)((BYTE*)Header + Header->e_lfanew);
		if (NtHeader->Signature == IMAGE_NT_SIGNATURE)
		{
			IMAGE_DATA_DIRECTORY* ImportDirectoryEntry = &NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

			IMAGE_IMPORT_DESCRIPTOR* ImportDescriptors = (IMAGE_IMPORT_DESCRIPTOR*)MapRvaToPointer(Header, NtHeader, ImportDirectoryEntry->VirtualAddress);
			for (size_t ImportIdx = 0; ImportIdx * sizeof(IMAGE_IMPORT_DESCRIPTOR) < ImportDirectoryEntry->Size; ImportIdx++)
			{
				IMAGE_IMPORT_DESCRIPTOR* ImportDescriptor = ImportDescriptors + ImportIdx;
				if (ImportDescriptor->Characteristics == 0 && ImportDescriptor->TimeDateStamp == 0 && ImportDescriptor->ForwarderChain == 0 && ImportDescriptor->Name == 0 && ImportDescriptor->FirstThunk == 0)
					break;

				if (ImportDescriptor->Name != 0)
				{
					const char* ImportName = (const char*)MapRvaToPointer(Header, NtHeader, ImportDescriptor->Name);
					ImportNames.Add(ImportName);
				}
			}

			return true;
		}
	}

	return false;
}


bool FWindowsPlatformProcess::ResolveImport(const FString& Name, const TArray<FString>& SearchPaths, FString& OutFileName)
{
	auto SearchPathsFunc = [ &OutFileName, &SearchPaths, &Name](int StartIdx, int EndIdx)
		{
			for (int32 i = StartIdx; i < EndIdx; ++i)
			{
				//TStringBuilder<MAX_PATH> FileName;
				// 
				//FPathViews::Append(FileName, SearchPaths[Idx], Name);

				FString FileName = FString::PrintF(TEXT("%s/%s"), SearchPaths[i], Name);


				if (FPaths::FileExists(*FileName))
				{
					OutFileName = FileName;
					return true;
				}
			}
			return false;
		};

	const int FirstAddedSearchPathIdx = 1 + ((DllDirectoryStack.Num() > 0) ? 1 : 0);
	if (SearchPathsFunc(0, FirstAddedSearchPathIdx))
	{
		return true;
	}
	return false;
}