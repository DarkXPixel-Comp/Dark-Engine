<<<<<<< Updated upstream
//#include "PackageManager.h"
//#include "libzippp.h"
//#include "Misc/Paths.h"
//#include "Containers/Array.h"
//
//static TArray<TUniquePtr<libzippp::ZipArchive>> Archives;
//
//
////void FPackageManager::OpenPackages()
//{
//	using namespace libzippp;
//	
//
////	Archives.Emplace(std::make_unique<ZipArchive>());
//
//
//
//
//
//}
=======
#include "PackageManager.h"
#include "libzippp.h"
#include "Misc/Paths.h"
#include "Containers/Array.h"
#include <filesystem>
#include "PackageFormat.h"

static TArray<TUniquePtr<libzippp::ZipArchive>> Archives;

using namespace libzippp;

void FPackageManager::OpenPackages()
{	
	if (USE_PACKAGE_CONTENT)
	{
		for (const auto& Entry : std::filesystem::recursive_directory_iterator(*FPaths::EngineContentDir()))
		{
			if (Entry.path().extension() == TEXT(".package"))
			{
				Archives.Emplace(std::make_unique<ZipArchive>(Entry.path().relative_path().string().c_str()));
			}
		}
	}


	// TODO: Ëó÷øå zlib



}

void FPackageManager::FindPackage(const FString& InName, FPackage& OutPackage)
{
	for (const auto& i : Archives)
	{
	//	i->
	}

}

void FPackageManager::ReadPackage(const TSharedPtr<class FPackage>& InPackage, void* OutData)
{



}
>>>>>>> Stashed changes
