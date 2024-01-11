#pragma once


#include "CoreMinimal.h"


class FEngineLoop
{
public:
	FEngineLoop() {}
	virtual ~FEngineLoop() {}

public:
	int32 PreInit(int32 ArgC, TCHAR* ArgV[], const TCHAR* AdditionalCommandLine = nullptr) { return 0; }
	int32 PreInit(const TCHAR* CmdLine) { return 0; }
	int32 PreInitPreStartupScreen(const TCHAR* CmdLine) { return 0; }
	
	int32 Init() { return 0; }
	void InitTime() {}
	void Exit() {}
	void Tick() {}





};


extern FEngineLoop GEngineLoop;

