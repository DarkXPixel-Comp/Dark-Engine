#include "Misc/Config.h"
#include "CoreGlobals.h"

void FConfigCache::InitConfigSystem()
{
	GConfig = new FConfigCache();

	bool bEngineConfigCreated = InitConfigFiles();



	GConfig->bReady = bEngineConfigCreated;
}

bool FConfigCache::InitConfigFiles()
{
	return false;
}

FConfigCache::FConfigCache() : bReady(false)
{

}
