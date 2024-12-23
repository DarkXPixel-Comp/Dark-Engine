#pragma once
#include "Platform/Platform.h"
#include "Math/MathFwd.h"
#include "Math/Vector2.h"

struct FGeometry
{
public:
	DarkUI_API FGeometry();

	FGeometry(const FGeometry& RHS) = default;

	FGeometry(const FVector2f& OffsetFromParent, 
		const FVector2f& ParentAbsolutePosition, 
		const FVector2f& InLocalSize,
		float InScale);


private:
};