#include "SceneRendering.h"
#include "IndirectLightRendering.h"
#include "GlobalShader.h"

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

	//RenderQuad(CmdList);
	RenderFractal(CmdList);







}


void FViewInfo::Init()
{
	ViewRect = UnscaledRect;
	ViewRect = FIntRect(FIntPoint(0, 0), RenderTarget->GetSizeXY());
	//ViewRect = FIntRect(0, 0, 1920, 1080);
	ShaderMap = GGlobalShaderMap;
}
