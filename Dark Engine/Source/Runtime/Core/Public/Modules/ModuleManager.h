#pragma once
#include "Platform/Platform.h"
#include "Containers/DarkString.h"
#include "Module.h"
#include "Memory/MemoryCore.h"
#include "Misc/Delegate.h"
#include <map>




class FModuleManager
{
	struct FModuleInfo
	{
		FString OriginalFilename;
		FString FileName;
		void* Handle = nullptr;
		TUniquePtr<IModule> Module;
		bool bIsReady = false;
	};

	typedef TSharedPtr<FModuleInfo> ModuleInfoPtr;

public:
	DECLARE_DELEGATE_RetVal(IModule*, FInitStaticModule);

	static CORE_API	FModuleManager& Get();


	CORE_API ModuleInfoPtr AddModule(const FString& InModuleName);

	CORE_API IModule* GetModule(const FString& InModuleName);

	CORE_API bool IsModuleLoaded(const FString& InModuleName) const;

	CORE_API IModule* LoadModule(const FString& InModuleName);

	CORE_API IModule* GetOrLoadModule(const FString& InModuleName);

	CORE_API void AddModuleToModulesList(const FString& InModuleName, TSharedPtr<FModuleManager::FModuleInfo>& ModuleInfo);

	CORE_API void RegisterStaticModule(const FString& InModuleName, const FInitStaticModule& InInitilizer)
	{
		PendingStaticModuleInitializers.Emplace(InModuleName, InInitilizer);
	}

	template<class TModuleInterface>
	static TModuleInterface* LoadModulePtr(const FString& InModuleName)
	{
		return static_cast<TModuleInterface*>(FModuleManager::Get().LoadModule(InModuleName));
	}

public:
	CORE_API ModuleInfoPtr FindModule(const FString& InModuleName) const;
	void ProcessPendingStaticInitializers();

	void FindModulePaths(const TCHAR* Pattern, std::unordered_map<FString, FString>& OutModulePaths) const;

	void FindModulePathsInDirectory(const FString& InDirectory, bool bIsGameDirectory, std::unordered_map<FString, FString>& OutModulePaths) const;


private:
	TArray<std::pair<FString, FInitStaticModule>> PendingStaticModuleInitializers;
	std::unordered_map<FString, FInitStaticModule> StaticModuleInitializers;

	std::unordered_map<FString, TSharedPtr<FModuleInfo>> ModuleMap;

	mutable std::unordered_map<FString, FString> ModulePathsCache;
};

template<class ModuleClass>
class FStaticModuleRegistrant
{
public:
	FStaticModuleRegistrant(const FString& InModuleName)
	{
		FModuleManager::FInitStaticModule Initilizer =
			FModuleManager::FInitStaticModule::Create(this, &FStaticModuleRegistrant<ModuleClass>::InitializeModule);

		FModuleManager::Get().RegisterStaticModule(InModuleName, Initilizer);
	}


	IModule* InitializeModule()
	{
		return new ModuleClass;
	}

};


class FDynamicModuleRegistrant
{
public:
	typedef IModule* (*FInitializerModuleFunction)(void);

	CORE_API FDynamicModuleRegistrant(const FString& InName, FInitializerModuleFunction InFunction);
	CORE_API ~FDynamicModuleRegistrant();

	static FInitializerModuleFunction FindModule(const FString& Name);

private:
	FInitializerModuleFunction Function;
	FString Name;
};



#define IMPLEMENT_STATIC_MODULE(ModuleClass, ModuleName) \
	static FStaticModuleRegistrant<ModuleClass> ModuleRegistrant##ModuleName(TEXT(#ModuleName)); \
	extern "C" void IMPLEMENT_MODULE_##ModuleName() {}
	

#define IMPLEMENT_DYNAMIC_MODULE (ModuleClass, ModuleName)	\
	static IModule* Initialize##ModuleName##Module() \
	{ return new ModuleClass(); } \
	static FDynamicModuleRegistrant ModuleRegistrant##ModuleName(TEXT(#ModuleName), Initialize##ModuleName##Module);  \
	extern "C" void IMPLEMENT_MODULE_##ModuleName() {}