#pragma once
#include "ScenePrivate.h"
#include "SceneView.h"
#include "RendererInterface.h"
#include "RHICommandList.h"
#include "SceneTextures.h"




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



	void RenderTest(FRHICommandListImmediate& CmdList);
	void RenderQuad(FRHICommandListImmediate& CmdList);
	void RenderFractal(FRHICommandListImmediate& CmdList);
	void RenderSeaScape(FRHICommandListImmediate& CmdList);


	void RenderPrePass(FRHICommandListImmediate& CmdList, TRefCountPtr<FRHITexture> SceneDepthTexture);

	void RenderTextureQuad(FRHICommandListImmediate& CmdList, TRefCountPtr<FRHITexture> InTexture);

	void FXAA(FRHICommandListImmediate& CmdList);


	static FSceneTextures SceneTextures;

	FScene* Scene;
	FViewInfo* SceneView;

};

