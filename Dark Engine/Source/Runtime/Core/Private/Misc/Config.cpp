#include "Misc/Config.h"
#include "CoreGlobals.h"
#include "Misc/Paths.h"
#include "Console/GlobalInputOutConsole.h"
#include <fstream>		
#include <sstream>


void ParseConfigLine(const FString& InLine, FString& OutVar, FString& OutValue)
{
	std::wstringstream Stream(InLine.GetNativeString());

	std::getline(Stream, OutVar.GetNativeString(), TEXT('='));
	std::getline(Stream, OutValue.GetNativeString());
}

void FConfigCache::InitConfigSystem()
{
	GConfig = new FConfigCache();

	bool bEngineConfigCreated = InitConfigFiles();



	GConfig->bReady = bEngineConfigCreated;
}

bool FConfigCache::InitConfigFiles()
{
	const TCHAR* DefaultConfigFiles[] =
	{
		TEXT("Engine.ini")
	};

	FString DefaultPath = FPaths::ConfigDir();

	for (int32 i = 0; i < DE_ARRAY_COUNT(DefaultConfigFiles); ++i)
	{
		FString Path = DefaultPath / DefaultConfigFiles[i];
		std::wifstream File(*Path);
		if (File.is_open())
		{
			TCHAR Buffer[256] = TEXT("\0");
			while (File.getline(Buffer, 256, TEXT('\n')))
			{
				FString Var;
				FString Value;
				ParseConfigLine(Buffer, Var, Value);
				if (Var.Len() && Value.Len())
				{
					GGlobalConsole.RegisterDefaultValue(*Var, Value);
				}
				FMemory::Memzero(Buffer, 256);
			}
			File.close();
		}
	}



	return true;
}

FConfigCache::FConfigCache() : bReady(false)
{

}
