#include "SceneRendering.h"

FSceneRender::FSceneRender(const FSceneView* InView) :
	Scene(InView->Scene->GetRenderScene()),
	SceneView(*InView)
{
	SceneView.SetSceneRenderer(this);


}

void FSceneRender::Render(FRHICommandListImmediate& CmdList)
{
	RenderTriangle(CmdList);







}

void FSceneRender::RenderTriangle(FRHICommandListImmediate& CmdList)
{
	//CmdList.






}

