#pragma once
#include "Canvas.h"
#include "SceneInterface.h"
#include "Engine/EditorViewportClient.h"



class FSceneView
{
public:
	FSceneView(const class FRenderTarget* InRenderTarget, FSceneInterface* InSceneInterface, FEditorViewportClient* InViewportClient);
	const class FRenderTarget* RenderTarget;

	void CalcSceneView();


public:
	FMatrix ProjectionMatrix;
	FIntRect ViewRect;

private:
	FEditorViewportClient* ViewportClient;



};