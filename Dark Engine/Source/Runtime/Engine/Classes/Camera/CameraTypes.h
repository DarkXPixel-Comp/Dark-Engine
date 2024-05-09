#pragma once
#include "CoreMinimal.h"
#include "CoreGlobals.h"



struct FMinimalViewInfo
{
	FVector Location;

	FRotator Rotation;

	float FOV;

	float PerspectiveNearClipPlane;

	float AspectRatio;


	FMatrix CalculateProjectionMatrix() const;

	static void CalculateProjectionMatrixGivenView(FMinimalViewInfo& ViewInfo, class FViewport* Viewport,class FSceneViewProjectionData& InOutProjectionData);
	static void CalculateProjectionMatrixGivenViewRectangle(FMinimalViewInfo& ViewInfo, const FIntRect& ConstrainedViewRectangle, class FSceneViewProjectionData& InOutProjectionData);


	float GetPerspectiveNearClipPlane() const
	{
		return PerspectiveNearClipPlane > 0.f ? PerspectiveNearClipPlane : GNearClippingPlane;
	}



};