#include "GenericPlatform/GenericPlatformMisc.h"
#include "Templates/DarkTemplate.h"



void MakeEngineDir(FString& OutEngineDir)
{
	FString DefaultEngineDir = /*TEXT*/("../../../Engine/");

	//const TCHAR* BaseDir = FPlatformProcess::BaseDir();


	OutEngineDir = MoveTemp(DefaultEngineDir);

}