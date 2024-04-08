#pragma once
#include "ScenePrivate.h"
#include "SceneView.h"
#include "RendererInterface.h"
#include "RHICommandList.h"


class FViewInfo : public FSceneView
{
public:
	FViewInfo(const FSceneView* InView) :
		FSceneView(*InView)
	{
		Init();
	}
	void Init();


	FIntRect ViewRect;
	class FGlobalShaderMap* ShaderMap;
};


class FSceneRender : public ISceneRenderer
{
public:
	FSceneRender(const FSceneView* InView);
	~FSceneRender();


public: 
	void Render(FRHICommandListImmediate& CmdList);
	void Flush() {}
	//void RenderUI(class FRenderTarget* RenderTarget) {}


	void RenderTriangle(FRHICommandListImmediate& CmdList);



	FScene* Scene;
	FViewInfo* SceneView;

};

