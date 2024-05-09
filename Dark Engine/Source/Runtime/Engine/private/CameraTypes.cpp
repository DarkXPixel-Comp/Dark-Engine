#include "Camera/CameraTypes.h"
#include "SceneView.h"
#include "DarkClient.h"
#include "Math/PersperctiveMatrix.h"


FMatrix FMinimalViewInfo::CalculateProjectionMatrix() const
{
	FMatrix ProjectionMatrix;

	const float ClippingPlane = GetPerspectiveNearClipPlane();
	ProjectionMatrix = FReversedZPerspectiveMatrix(FMath::Max(0.001f, FOV) * (float)DE_PI / 360.f, AspectRatio, 1.f, ClippingPlane);

	return ProjectionMatrix;
}

void FMinimalViewInfo::CalculateProjectionMatrixGivenView(FMinimalViewInfo& ViewInfo,FViewport* Viewport, FSceneViewProjectionData& InOutProjectionData)
{
	FIntRect ViewExtents = Viewport->CalculateViewExtents(ViewInfo.AspectRatio, InOutProjectionData.ViewRect);
	CalculateProjectionMatrixGivenViewRectangle(ViewInfo, ViewExtents, InOutProjectionData);
}

void FMinimalViewInfo::CalculateProjectionMatrixGivenViewRectangle(FMinimalViewInfo& ViewInfo, const FIntRect& ConstrainedViewRectangle, FSceneViewProjectionData& InOutProjectionData)
{
	bool bOrthographic = false;

	InOutProjectionData.ConstrainedViewRect = ConstrainedViewRectangle;
	InOutProjectionData.ProjectionMatrix = ViewInfo.CalculateProjectionMatrix();
}