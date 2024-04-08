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
	RenderLight();

	//RenderTriangle(CmdList);







}

void FSceneRender::RenderTriangle(FRHICommandListImmediate& CmdList)
{
	//CmdList.






}

void FViewInfo::Init()
{
	ViewRect = FIntRect(0, 0, 0, 0);
	ShaderMap = GGlobalShaderMap;
}
