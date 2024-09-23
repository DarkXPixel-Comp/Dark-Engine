#include "CoreMinimal.h"
#include "LaunchEngineLoop.h"

#include "Misc/CoreDelegates.h"
#include "CoreGlobals.h"


FEngineLoop GEngineLoop;



int32 EnginePreInit(const TCHAR* CmdLine)
{
	return GEngineLoop.PreInit(CmdLine);
}

int32 EngineInit(const TCHAR* CmdLine)
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
	FCoreDelegates::OnPreMain.BroadCast();
	int32 Error = EnginePreInit(CmdLine);

	if (Error != 0 || IsExitRequested())
	{
		return Error;
	}
	
	Error = EngineInit(CmdLine);

	while (!IsExitRequested())
	{
		EngineTick();
	}

	
	EngineExit();
	return Error;
}