#pragma once
#include "Containers/DarkString.h"
#include "Platform/Platform.h"


struct FGenericWindowDefinition
{
	FString Title;
	
	float PosX = 0.f;
	float PosY = 0.f;

	float Width = 0.f;
	float Height = 0.f;

	bool bWindowBorder = true;
	int32 CornerRadius = 5;
};