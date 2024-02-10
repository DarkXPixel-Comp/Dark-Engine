#pragma once
#include "CoreMinimal.h"
#include "Rendering/RenderingComon.h"


class UIViewport
{
public:
	UIViewport();
	FVector2D ViewportSize;




protected:
	TWeakPtr<IUIViewport> ViewportInterface;






};