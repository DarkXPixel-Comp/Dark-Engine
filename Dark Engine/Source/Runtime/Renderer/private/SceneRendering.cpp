#include "SceneRendering.h"
#include "IndirectLightRendering.h"

FSceneRender::FSceneRender(const FSceneView* InView) :
	Scene(InView->Scene ? InView->Scene->GetRenderScene() : nullptr),
	SceneView(*InView)
{
	//SceneView.SetSceneRenderer(this);


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

