#pragma once
#include "CoreMinimal.h"
#include "Rendering/RenderingCommon.h"


class UIViewport
{
public:
	UIViewport();
	FVector2D ViewportSize;




protected:
	TWeakPtr<IUIViewport> ViewportInterface;






};