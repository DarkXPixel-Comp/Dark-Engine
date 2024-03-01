#pragma once
#include "Canvas.h"
#include "SceneInterface.h"
#include "Engine/EditorViewportClient.h"
#include "RendererInterface.h"
#include "SceneView.h"
#include "DarkClient.h"




class FSceneView
{
public:
	FSceneView(const FRenderTarget* InRenderTarget, FSceneInterface* InSceneInterface, FEditorViewportClient* InViewportClient);
	void CalcSceneView();
	void SetSceneRenderer(ISceneRenderer* InSceneRenderer) { SceneRenderer = InSceneRenderer; }
	ISceneRenderer* GetSceneRenderer() const { return SceneRenderer; }



public:
	FMatrix ProjectionMatrix;
	FIntRect ViewRect;
	FSceneInterface* Scene;
	uint32 FrameNumber;
	uint64 FrameCounter = 0;
	const FRenderTarget* RenderTarget;

private:
	FEditorViewportClient* ViewportClient;
	ISceneRenderer* SceneRenderer;



};