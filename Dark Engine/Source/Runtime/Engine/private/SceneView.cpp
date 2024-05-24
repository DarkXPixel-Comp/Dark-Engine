#include "SceneView.h"



FSceneView::FSceneView(const FRenderTarget* InRenderTarget, FSceneInterface* InSceneInterface):
	RenderTarget(InRenderTarget)
{

}

void FSceneView::Init(const FSceneViewInitOptions& InitOptions)
{
	ViewInitOptions = InitOptions;
	ViewLocation = InitOptions.ViewLocation;
	ViewRotation = InitOptions.ViewRotation;

	FOV = InitOptions.FOV;
	DesiredFOV = InitOptions.FOV;
}


void FSceneView::CalcSceneView()
{	








}