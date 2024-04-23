#pragma once


#include "CoreMinimal.h"

/**
*	@author DarkXPixel - https:darkengine.com - <EMAIL> - nahmurin.dima65@gmail.com
*	@brief The engine loop class.
*	@version 1.0
*	@date 23/04/2024
*/
class FEngineLoop
{
public:
	FEngineLoop() {}
	virtual ~FEngineLoop() {}

public:
	int32 PreInit(const TCHAR* CmdLine);

	
	
	int32 Init();
	void InitTime() {}
	void Exit();
	void Tick();





};


extern FEngineLoop GEngineLoop;

