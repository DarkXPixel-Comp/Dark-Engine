#pragma once
#include <unordered_map>
#include "Containers/DarkString.h"
#include "Memory/MemoryCore.h"




class FPackageManager
{
	friend class FPackage;
public:
	static FPackageManager& Get()
	{
		static FPackageManager Manager;
		return Manager;
	}


private:
	FPackageManager()
	{
		OpenPackages();
	}

	void FindPackage(const FString& InName, class FPackage& OutPackage);

private:
	void OpenPackages();
	void ReadPackage(const TSharedPtr<class FPackage>& InPackage, void* OutData);







private:
	std::unordered_map<FString, TSharedPtr<class FPackage>> Packages;
	std::unordered_map<FString, std::vector<int8>> CachePackages;

};