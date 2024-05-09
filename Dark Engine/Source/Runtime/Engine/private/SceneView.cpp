#include "SceneView.h"



FSceneView::FSceneView(const FRenderTarget* InRenderTarget, FSceneInterface* InSceneInterface):
	RenderTarget(InRenderTarget)
{

}

void FSceneView::Init(const FSceneViewInitOptions& InitOptions)
{
	ViewInitOptions = InitOptions;

	FOV = InitOptions.FOV;
}


void FSceneView::CalcSceneView()
{	








}