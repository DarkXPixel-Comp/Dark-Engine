#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "Platform/PlatformProcess.h"
#include <filesystem>

//CORE_API std::unordered_map<FString, FDynamicModuleRegistrant*> GMapDynamicModuleRegistrants;
FDynamicModuleRegistrant* GFirstModuleRegistrant;

FModuleManager& FModuleManager::Get()
{
	static FModuleManager ModuleManager;
	return ModuleManager;

}

CORE_API FModuleManager::ModuleInfoPtr FModuleManager::AddModule(const FString& InModuleName)
{
	if (ModuleMap.contains(InModuleName))
	{
		return nullptr;
	}

	TSharedPtr<FModuleInfo> ModuleInfo = MakeShareble(new FModuleInfo());

	AddModuleToModulesList(InModuleName, ModuleInfo);
	return ModuleInfo;
}

CORE_API IModule* FModuleManager::GetModule(const FString& InModuleName)
{
	return nullptr;
}

CORE_API IModule* FModuleManager::LoadModule(const FString& InModuleName)
{
	return GetOrLoadModule(InModuleName);
}

CORE_API IModule* FModuleManager::GetOrLoadModule(const FString& InModuleName)
{
	IModule* LoadedModule = nullptr;

	ModuleInfoPtr FoundModulePtr = FindModule(InModuleName);
	if (FoundModulePtr)
	{
		LoadedModule = FoundModulePtr->Module.get();
		if (LoadedModule)
		{
			return LoadedModule;
		}
	}
	else
	{
		FoundModulePtr = AddModule(InModuleName);
	}

	ProcessPendingStaticInitializers();

	auto it = StaticModuleInitializers.find(InModuleName);
	const FInitStaticModule* ModuleInitalizer = it == StaticModuleInitializers.end() ? nullptr : &it->second;
	if (ModuleInitalizer)
	{
		FoundModulePtr->Module = MakeUnique(ModuleInitalizer->BroadCast());

		if (FoundModulePtr->Module)
		{
			FoundModulePtr->Module->StartModule();
		}
		FoundModulePtr->bIsReady = true;
		LoadedModule = FoundModulePtr->Module.get();
	}
	else
	{
		if (FoundModulePtr->FileName.Lenght() == 0 || !FPaths::FileExists(FoundModulePtr->FileName))
		{
			std::unordered_map<FString, FString> ModulePathMap;
			FindModulePaths(*InModuleName, ModulePathMap);
			if (ModulePathMap.size() != 1)
			{
				return nullptr;
			}
			FoundModulePtr->FileName = ModulePathMap.begin()->second;
		}

		FoundModulePtr->Handle = nullptr;
		const FString ModuleFile = FoundModulePtr->FileName;

		if (FPaths::FileExists(ModuleFile))
		{
			FoundModulePtr->Handle = FPlatformProcess::GetDllHandle(*ModuleFile);
		}

		if (FoundModulePtr->Handle != nullptr)
		{
			FDynamicModuleRegistrant::FInitializerModuleFunction InitializerFunction = FDynamicModuleRegistrant::FindModule(InModuleName);
			if (!InitializerFunction)
			{
				InitializerFunction = (FDynamicModuleRegistrant::FInitializerModuleFunction)FPlatformProcess::GetDllExport(FoundModulePtr->Handle, TEXT("InitializeModule"));
			}

			if (InitializerFunction)
			{
				if (FoundModulePtr->Module)
				{
					LoadedModule = FoundModulePtr->Module.get();
				}
				else
				{
					if (FoundModulePtr->Module = MakeUnique<IModule>(InitializerFunction()))
					{
						FoundModulePtr->Module->StartModule();

						FoundModulePtr->bIsReady = true;
						LoadedModule = FoundModulePtr->Module.get();
					}
					else
					{
						FPlatformProcess::FreeDllHandle(FoundModulePtr->Handle);
					}
				}
				
			}
		}


	}

	return LoadedModule;
}

CORE_API void FModuleManager::AddModuleToModulesList(const FString& InModuleName, TSharedPtr<FModuleManager::FModuleInfo>& ModuleInfo)
{
	ModuleMap.emplace(InModuleName, ModuleInfo);
}

CORE_API FModuleManager::ModuleInfoPtr FModuleManager::FindModule(const FString& InModuleName) const
{
	auto it = ModuleMap.find(InModuleName);
	return it == ModuleMap.end() ? nullptr : it->second;
}

void FModuleManager::ProcessPendingStaticInitializers()
{
	for (auto& i : PendingStaticModuleInitializers)
	{
		StaticModuleInitializers.emplace(i);
	}

	PendingStaticModuleInitializers.Empty();
}

void FModuleManager::FindModulePaths(const TCHAR* Pattern, std::unordered_map<FString, FString>& OutModulePaths) const
{
	if (ModulePathsCache.size() == 0)
	{
		FindModulePathsInDirectory(FPlatformProcess::GetModulesDirectory(), false, ModulePathsCache);
	}

	if (Pattern == nullptr || Pattern == FString(TEXT("*")))
	{
		OutModulePaths = ModulePathsCache;
	}
	else
	{
		for (const auto& Pair : ModulePathsCache)
		{
			if (Pair.first == Pattern/*Pair.first.ContainsWithoutCase(Pattern)*/)
			{
				OutModulePaths.emplace(Pair);
			}
		}
	}
}

void FModuleManager::FindModulePathsInDirectory(const FString& InDirectory, bool bIsGameDirectory, std::unordered_map<FString, FString>& OutModulePaths) const
{
	if (std::filesystem::is_directory(*InDirectory))
	{
		//TArray<FString> SearchDirectoryNames;


		for (const auto& Entry : std::filesystem::recursive_directory_iterator(*InDirectory))
		{
			if (std::filesystem::is_regular_file(Entry.status()))
			{
				//SearchDirectoryNames.Add(Entry.path().generic_wstring());
				OutModulePaths.emplace(Entry.path().filename().stem().generic_wstring(), Entry.path().generic_wstring());
			}
		}
	}
}

FDynamicModuleRegistrant::FDynamicModuleRegistrant(const FString& InName, FInitializerModuleFunction InFunction) :
	Name(InName),
	Function(InFunction)
{
	//GMapDynamicModuleRegistrants.emplace(InName, this);
	
	Prev = nullptr;
	Next = GFirstModuleRegistrant;

	if (GFirstModuleRegistrant)
		GFirstModuleRegistrant->Prev = this;

	GFirstModuleRegistrant = this;

}

FDynamicModuleRegistrant::~FDynamicModuleRegistrant()
{
	//GMapDynamicModuleRegistrants.erase(Name);

	if (Next)
		Next->Prev = Prev;
	if (Prev)
		Prev->Next = Next;
	else
	{
		GFirstModuleRegistrant = Next;
	}
}

FDynamicModuleRegistrant::FInitializerModuleFunction FDynamicModuleRegistrant::FindModule(const FString& Name)
{
	//auto it = GMapDynamicModuleRegistrants.find(Name);
	//return it == GMapDynamicModuleRegistrants.end() ? nullptr : it->second->Function;

	for (FDynamicModuleRegistrant* Entry = GFirstModuleRegistrant; Entry; Entry = Entry->Next)
	{
		if (Name == Entry->Name)
			return Entry->Function;
	}
	return nullptr;
}
