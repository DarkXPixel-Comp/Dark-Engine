#include "LaunchEngineLoop.h"
#include "Misc/Paths.h"
#include "Templates/DarkTemplate.h"
#include "Platform/PlatformMisc.h"
#include "Console/BaseConsole.h"
#include "CoreGlobals.h"
#include <vector>
#include <filesystem>
#include <fstream>
#include "PackageManager.h"
#include "optick.h"


static const FString VitalFolders[] = { "Content", "Config", "Shaders", "Binaires" };

static int32 CheckFiles(FString& AdditionalErrorMsg)
{
	int32 AllFilesIsOk = false;
	int32 NumFiles = 0;

	std::vector<FString> TempFolders(std::begin(VitalFolders), std::end(VitalFolders));

	for (const auto& Entry : std::filesystem::directory_iterator(*FPaths::EngineDir()))
	{
		auto& Path = Entry.path();
		auto str = Path.filename().wstring();

		auto Result = std::find(std::begin(VitalFolders), std::end(VitalFolders), Path.filename().wstring());

		if (Result != std::end(VitalFolders))
		{
			++NumFiles;
			TempFolders.erase(std::find(TempFolders.begin(), TempFolders.end(), *Result));
		}
	}


	for (auto& LostFile : TempFolders)
	{
		AdditionalErrorMsg += (LostFile + TEXT("\n"));
	}


	return NumFiles == DE_ARRAY_COUNT(VitalFolders);

}



int32 FEngineLoop::PreInit(const FString& CmdLine)
{
	OPTICK_EVENT("PreInit");
	FString ErrorMsg;
	if (!CheckFiles(ErrorMsg))
	{
		FPlatformMisc::CreateMessageBoxError(*FString::PrintF(TEXT("Not found all vital files or folders: \n%s"), *ErrorMsg), TEXT("Error vital files"));
		RequestExit();
		return -1;
	}

	return 0;
}

int32 FEngineLoop::Init()
{
	return 0;
}

void FEngineLoop::Tick()
{
}

void FEngineLoop::Exit()
{
	bExit = true;
}
