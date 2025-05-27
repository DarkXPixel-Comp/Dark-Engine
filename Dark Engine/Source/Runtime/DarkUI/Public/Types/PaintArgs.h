#pragma once
#include "Platform/Platform.h"	
#include "Math/Vector2.h"


class FHittestGrid;
class DUIWidget;



class FPaintArgs
{
public:
	DarkUI_API FPaintArgs(const DUIWidget* PaintParent, FHittestGrid& InRootHittestGrid, FHittestGrid& InCurrentHitTestGrid,const FVector2f& InWindowOffset, float InDeltaTime);



private:
	FHittestGrid& RootGrid;
	FHittestGrid& CurrentGrid;
	FVector2f WindowOffset;

	double CurrentTime;
	float DeltaTime;
	bool bInherittedHittestability = true;
	bool bDefferedPainting = true;
};