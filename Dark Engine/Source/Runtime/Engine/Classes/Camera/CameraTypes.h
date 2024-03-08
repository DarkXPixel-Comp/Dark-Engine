#pragma once
#include "CoreMinimal.h"



struct FMinimalViewInfo
{
	FVector Location;

	FRotator Rotation;

	float FOV;

	float NearClipPlane;

	float AspectRatio;


	FMatrix CalculateProjectionMatrix() const;

	void CalculateProjectionMatrixGivenViewRectangle(const FMinimalViewInfo& ViewInfo, class FSceneView& InOutProjectionData);




};