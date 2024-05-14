#include "SceneRendering.h"
#include "IndirectLightRendering.h"
#include "GlobalShader.h"
#include "CoreGlobals.h"

FSceneRender::FSceneRender(const FSceneView* InView) :
	Scene(InView->Scene ? InView->Scene->GetRenderScene() : nullptr)
{
	SceneView = new FViewInfo(InView);
}

FSceneRender::~FSceneRender()
{
	delete SceneView;
}

void FSceneRender::Render(FRHICommandListImmediate& CmdList)
{
	//RenderLight();

	switch (GRenderMode)
	{
	case 0:
		RenderQuad(CmdList);
		break;
	case 1:
		RenderFractal(CmdList);
		break;
	case 2:
		RenderSeaScape(CmdList);
		break;
	default:
		RenderQuad(CmdList);
		break;
	}

	if(GFXAAEnable)
		FXAA(CmdList);
}


void FViewInfo::Init()
{
	ViewRect = UnscaledRect;
	ViewRect = FIntRect(FIntPoint(0, 0), RenderTarget->GetSizeXY());
	//ViewRect = FIntRect(0, 0, 1920, 1080);
	ShaderMap = GGlobalShaderMap;
}
