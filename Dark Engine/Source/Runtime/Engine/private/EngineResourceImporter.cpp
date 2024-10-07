#include "EngineResourceImporter.h"
#include <filesystem>

TArray<TSharedPtr<IFormatEngineResourceImporter>> FEngineResourceImporter::DefaultImporters;

FEngineResourceImporter::FEngineResourceImporter() : ImporterPool(1)
{

}

std::future<TArray<TObjectPtr<GObject>>> FEngineResourceImporter::ImportObjects(const FString& Path)
{
	FString Extension = std::filesystem::path(Path.GetNativeString()).extension().wstring();
	IFormatEngineResourceImporter* Importer = nullptr;

	for (auto& i : DefaultImporters)
	{
		if (i->GetAllSupportedFormats().Has(Extension))
		{
			Importer = i.get();
			break;
		}
	}
	if (Importer == nullptr)
	{
		return std::future<TArray<TObjectPtr<GObject>>>();
	}

	auto task = ImporterPool.submit_task([Importer, Path]() -> TArray<TObjectPtr<GObject>>
		{
			TSharedPtr<IFormatEngineResourceImporter> TaskImporter = Importer->CreateImporter();

			return TaskImporter->ImportAllObjects(Path);
		});


	return task;
}

void FEngineResourceImporter::WaitAllImports()
{
	ImporterPool.wait();
}
