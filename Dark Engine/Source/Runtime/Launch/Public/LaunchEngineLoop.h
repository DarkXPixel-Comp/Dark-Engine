#pragma once
#include "Platform/Platform.h"
#include "Containers/DarkString.h"

class FEngineLoop
{
public:
	FEngineLoop() {}
	~FEngineLoop()
	{
		if (!bExit)
		{
			Exit();
		}
	}

public:
	int32 PreInit(const FString& CmdLine);
	int32 Init();
	void Tick();
	void Exit();

private:
	bool bExit = false;
};


extern FEngineLoop GEngineLoop;