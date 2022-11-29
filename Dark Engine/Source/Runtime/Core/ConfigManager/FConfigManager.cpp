#include "../CoreMinimal.h"

#include <fstream>

FConfigManager* FConfigManager::inst;

FConfigManager::FConfigManager()
{
	inst = this;

	std::ifstream fin;

	try
	{
		fin.open("");
	}
	catch (const std::exception& ex)
	{

	}

}