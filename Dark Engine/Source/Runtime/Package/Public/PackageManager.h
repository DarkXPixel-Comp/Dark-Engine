#pragma once
#include <unordered_map>
#include "Containers/DarkString.h"
#include "Memory/MemoryCore.h"




class FPackageManager
{
public:
	FPackageManager& Get() const
	{
		static FPackageManager Manager;
		return Manager;
	}


private:
	FPackageManager()
	{
		OpenPackages();
	}


private:
	void OpenPackages();







private:
	std::unordered_map<FString, TSharedPtr<class FPackage>> Packages;
};