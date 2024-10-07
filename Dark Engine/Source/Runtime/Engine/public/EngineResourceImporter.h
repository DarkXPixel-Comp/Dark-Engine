#pragma once
#include "Object.h"
#include "ObjectPtr.h"
#include "Containers/Array.h"
#include "Containers/DarkString.h"
#include "Memory/TUniquePtr.h"
#include "BS_thread_pool.hpp"
#include <future>


class IFormatEngineResourceImporter
{
public:
	virtual TArray<TObjectPtr<GObject>>	ImportAllObjects(const FString& Path) = 0;
	virtual TArray<FString>	GetAllSupportedFormats() const = 0;
	virtual TSharedPtr<IFormatEngineResourceImporter> CreateImporter() = 0;


};

class FEngineResourceImporter
{
public:
	FEngineResourceImporter();

	std::future<TArray<TObjectPtr<GObject>>> ImportObjects(const FString& Path);

	void WaitAllImports();


	static void RegisterNewImporter(TSharedPtr<IFormatEngineResourceImporter> NewImporter)
	{
		DefaultImporters.Add(NewImporter);
	}



private:
	static TArray<TSharedPtr<IFormatEngineResourceImporter>> DefaultImporters;
	BS::thread_pool ImporterPool;

};