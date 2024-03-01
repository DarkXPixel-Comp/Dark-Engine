#pragma once
#include "ScenePrivate.h"
#include "SceneView.h"
#include "RendererInterface.h"
#include "RHICommandList.h"


class FSceneRender : public ISceneRenderer
{
public:
	FSceneRender(const FSceneView* InView);


public: 
	void Render(FRHICommandListImmediate& CmdList);
	void Flush() {}
	//void RenderUI(class FRenderTarget* RenderTarget) {}


	void RenderTriangle(FRHICommandListImmediate& CmdList);



	FScene* Scene;
	FSceneView SceneView;

};

