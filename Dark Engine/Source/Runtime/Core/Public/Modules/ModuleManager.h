#pragma once
#include <Containers/DarkString.h>
#include "ModuleInterface.h"


class FModuleManager
{
public :
	static FModuleManager& Get();

	void AddModule(const FString ModuleName);

	IModuleInterface* GetModule(const FString ModuleName);

	bool IsModuleLoaded(const FString ModuleName);

	IModuleInterface* LoadModule(const FString ModuleName);

	int32 GetModuleCount();

private:
	class FModuleInfo
	{
	public:
		FString OriginalFilename;
		FString FileName;
		void* Handle;
	};







};