#pragma once
#include "Platform/Platform.h"
#include "Math/MathFwd.h"
#include "Math/Vector2.h"
#include "Math/Transform2.h"

struct FGeometry
{
public:
	DARKUI_API FGeometry();

	FGeometry(const FGeometry& RHS) = default;

	FGeometry(const FVector2f& OffsetFromParent, 
		const FVector2f& ParentAbsolutePosition, 
		const FVector2f& InLocalSize,
		float InScale);


private:
	FTransform2f RenderTransform;

	const bool bHasRenderTransform = true;
};