#pragma once


#include "CoreMinimal.h"


class FEngineLoop
{
public:
	FEngineLoop() {}
	virtual ~FEngineLoop() {}

public:
	int32 PreInit(const TCHAR* CmdLine);

	
	
	int32 Init();
	void InitTime() {}
	void Exit() {}
	void Tick() {}





};


extern FEngineLoop GEngineLoop;

