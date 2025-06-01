#include "GenericPlatform/GenericPlatformMisc.h"
#include "Platform/PlatformProcess.h"


namespace
{
	struct FStaticData
	{
		FString RootDir;
		FString LaunchDir;
		FString ProjectDir;
		FString EngineDir;
	};
	static FStaticData StaticData;
}



static void MakeEngineDir(FString& OutEngineDir)
{
	FString DefaultEngineDir = TEXT("../../../Dark Engine/");

	FPlatformProcess::SetCurrentWorkingDirectoryToBaseDir();

	const TCHAR* BaseDir = FPlatformProcess::BaseDir();

	FString DirToTry = FString(BaseDir) / DefaultEngineDir / TEXT("Binaires");
	if (std::filesystem::exists(*DirToTry))
	{
		OutEngineDir = std::move(DirToTry);
		return;
	}

	OutEngineDir = std::move(DefaultEngineDir);
}


FString FGenericPlatformMisc::EngineDir()
{
	FString& EngineDir = StaticData.EngineDir;
	if (EngineDir.Lenght() == 0)
	{
		MakeEngineDir(EngineDir);

	}
	return *EngineDir;
}

void FGenericPlatformMisc::LocalPrint(const TCHAR* Str)
{
#ifdef UNICODE
	wprintf(TEXT("%s"), Str);
#else
	printf("%s", Str);
#endif
}

void FGenericPlatformMisc::CacheLaunchDir()
{
	FString& LaunchDir = StaticData.LaunchDir;
	if (LaunchDir.Lenght() == 0)
	{
		LaunchDir = FPlatformProcess::GetCurrentWorkingDirectory() + TEXT("/");
	}
}

const TCHAR* FGenericPlatformMisc::RootDir()
{
	FString& Path = StaticData.RootDir;
	if (Path.Lenght() == 0)
	{
		FString TempPath = EngineDir();
		int32 chopPos = TempPath.Find(TEXT("/Dark Engine"));
		if (chopPos != -1)
		{
			TempPath = TempPath.Mid(0, chopPos);
		}
		else
		{
			TempPath = FPlatformProcess::BaseDir();

			if (TempPath.Back() == TEXT('/'))
			{
				TempPath = TempPath.Mid(0, TempPath.Lenght() - 1);
			}

			

			chopPos = TempPath.Find(TEXT("/Dark Engine/Binaires"));
			if (chopPos != -1)
			{
				TempPath = TempPath.Mid(0, chopPos);
			}
			else
			{
				chopPos = TempPath.Find(TEXT("/../Binaires"));
				if (chopPos != -1)
				{
					TempPath = TempPath.Mid(0, chopPos) + TEXT("../../");
				}
				
			}
		}

		Path = std::filesystem::canonical(*TempPath).generic_wstring();
	}
	return *Path;
}
