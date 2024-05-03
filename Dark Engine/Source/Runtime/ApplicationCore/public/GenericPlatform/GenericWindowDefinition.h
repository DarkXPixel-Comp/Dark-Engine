#pragma once
#include "CoreMinimal.h"




struct FGenericWindowDefinition
{
	FString Title;

	float XPositionOnScreen = 0;
	float YPositionOnScreen = 0;

	float WidthOnScreen = 0;
	float HeightOnScreen = 0;

	bool bHasWindowBorder = true;
	int32 CornerRadius = 5;
	

};