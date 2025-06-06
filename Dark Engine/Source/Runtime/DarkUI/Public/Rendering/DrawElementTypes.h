#pragma once
#include "CoreMinimal.h"
#include "DrawElements.h"
#include "Math/Color.h"
#include "Styling/DarkUIBrush.h"
//#include "Layout/PaintGeometry.h"

class FPaintGeometry;

class FDUIDrawElement
{
public:
	enum class ERoationSpace
	{
		RelativeToElement,
		RelativeToWorld
	};

	DARKUI_API static void MakeDebugQuad(FDUIWindowElementList& ElementList, uint32 InLayer, const FPaintGeometry& PaintGeometry, FLinearColor Tint);

	DARKUI_API static void MakeBox(FDUIWindowElementList& ElementList, uint32 InLayer, const FPaintGeometry& PaintGeometry, const FDUIBrush* InBrush, const FLinearColor& InTint);
};


class FDUITintableElement
{
public:
	FLinearColor Tint;

	FORCEINLINE void SetTint(const FLinearColor& InTint) { Tint = InTint; }
	FORCEINLINE FLinearColor GetTint() const { return Tint; }
};

class FDUIBoxElement : FDUIDrawElement
{




};