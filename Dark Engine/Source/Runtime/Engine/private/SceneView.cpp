#include "SceneView.h"



FSceneView::FSceneView(const FRenderTarget* InRenderTarget, FSceneInterface* InSceneInterface):
	RenderTarget(InRenderTarget)
{

}


void FSceneView::CalcSceneView()
{	


	//ViewRect = FIntRect(FIntPoint(0, 0), ViewportSize);

	/*const FViewportCameraTransform& ViewTransform = ViewportClient->GetViewTransform();

	FMinimalViewInfo& MainViewInfo = ViewportClient->MainViewInfo;

	MainViewInfo.Location = ViewTransform.GetLocation();
	MainViewInfo.Rotation = ViewTransform.GetRotation();
	MainViewInfo.FOV = ViewportClient->FOV;


	FIntPoint ViewportSize = ViewportClient->GameViewport->GetSize();
	ViewportSize.X = FMath::Max(ViewportSize.X, 1);
	ViewportSize.Y = FMath::Max(ViewportSize.Y, 1);

	ViewRect = FIntRect(FIntPoint(0, 0), ViewportSize);


	MainViewInfo.CalculateProjectionMatrix();
*/







}