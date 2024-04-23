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
	FSceneView(const FRenderTarget* InRenderTarget, FSceneInterface* InSceneInterface);
	void CalcSceneView();



public:
	const class FRenderTarget* RenderTarget = nullptr;
	FSceneInterface* Scene = nullptr;
	FIntRect UnscaledRect;




};