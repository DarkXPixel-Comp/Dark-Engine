#pragma once
#include "CoreMinimal.h"
#include "Rendering/DarkUILayoutTransform.h"


struct FPaintGeometry
{
public:
	FPaintGeometry() :
		DrawPosition(0.f, 0.f),
		DrawScale(1.f),
		DrawSize(0.f, 0.f),
		LocalSize(0.f, 0.f),
		bHasRenderTransform(false)
	{}

	FPaintGeometry(const FDUILayoutTransform& InLayoutTransform, const FTransform2f& InRenderTransform, const FVector2f& InLocalSize, bool bInHasRenderTransform) :
		DrawPosition(InLayoutTransform.GetTranslation()),
		DrawScale(InLayoutTransform.GetScale()),
		DrawSize(0.f, 0.f),
		LocalSize(InLocalSize),
		RenderTransform(InRenderTransform),
		bHasRenderTransform(bInHasRenderTransform)
	{ }



public:
	FVector2f DrawPosition;

	float DrawScale;

	FVector2f GetLocalSize() const { return LocalSize; }

	FTransform2f GetRenderTransform() const { return RenderTransform; }

	bool HasRenderTransform() const { return bHasRenderTransform; }

private:
	mutable FVector2f DrawSize;
	mutable FVector2f LocalSize;
	mutable FTransform2f RenderTransform;

	bool bHasRenderTransform : 1;
};