#include "LaunchEngineLoop.h"
#include "CoreGlobals.h"



FEngineLoop GEngineLoop;


int32 EnginePreInit(const FString& CmdLine)
{
	return GEngineLoop.PreInit(CmdLine);
}

int32 EngineInit()
{
	return GEngineLoop.Init();
}

void EngineTick()
{
	GEngineLoop.Tick();
}

void EngineExit()
{
	GEngineLoop.Exit();
}


int32 GuardedMain(const TCHAR* CmdLine)
{
	int32 Error = EnginePreInit(CmdLine);

	if (Error != 0 || IsExitRequested())
	{
		return Error;
	}

	Error = EngineInit();

	while(!IsExitRequested())
	{
		EngineTick();
	}

	EngineExit();

	return Error;
}