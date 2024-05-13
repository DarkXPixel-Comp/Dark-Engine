#pragma once
#include "Canvas.h"
#include "SceneInterface.h"
#include "Engine/EditorViewportClient.h"
#include "RendererInterface.h"
#include "SceneView.h"
#include "DarkClient.h"


struct FSceneViewProjectionData
{
	FVector ViewOrigin;
	FMatrix ViewRotationMatrix;
	FMatrix ProjectionMatrix;
	FIntRect ViewRect;
	FIntRect ConstrainedViewRect;
	FVector CameraToViewTarget;

};

struct FSceneViewInitOptions : FSceneViewProjectionData
{
	FVector ViewLocation;
	FRotator ViewRotation;
	FIntPoint CursorPosition;
	FVector2f NormalizeCursorPosition;

	float FOV;

	float UnitsToMetersScale = 1.f;
};


struct FViewMatrices
{
	FViewMatrices()
	{
		ProjectionMatrix = FMatrix::Identity;
		InvProjectionMatrix = FMatrix::Identity;
		ViewMatrix = FMatrix::Identity;
		InvViewMatrix = FMatrix::Identity;
		InvViewProjectionMatrix = FMatrix::Identity;
		ViewOrigin = FVector::ZeroVector;
		CameraToViewTarget = FVector::ZeroVector;
		ProjectionScale = FVector2D(0, 0);
		ScreenScale = 1.f;
	}

	FMatrix ProjectionMatrix;
	FMatrix InvProjectionMatrix;
	FMatrix ViewMatrix;
	FMatrix InvViewMatrix;
	FMatrix InvViewProjectionMatrix;
	FVector ViewOrigin;
	FVector CameraToViewTarget;
	FVector2D ProjectionScale;
	
	float ScreenScale;





};


class FSceneView
{
public:
	FSceneView(const FRenderTarget* InRenderTarget, FSceneInterface* InSceneInterface);

	void Init(const FSceneViewInitOptions& InitOptions);

	void CalcSceneView();



public:
	const class FRenderTarget* RenderTarget = nullptr;
	FSceneInterface* Scene = nullptr;
	FIntRect UnscaledRect;
	FSceneViewInitOptions ViewInitOptions;

	FVector ViewLocation;
	FRotator ViewRotation;
	float UnitsToMetersScale;

	FViewMatrices ViewMatrices;


	float FOV;
	float DesiredFOV;
	float NearClippingDistance;


};