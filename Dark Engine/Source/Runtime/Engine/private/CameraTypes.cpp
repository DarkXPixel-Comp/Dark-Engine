#include "Camera/CameraTypes.h"
#include "SceneView.h"


FMatrix FMinimalViewInfo::CalculateProjectionMatrix() const
{
	FMatrix ProjectioMatrix;

	const float ClippingPlane = NearClipPlane;

//	ProjectioMatrix = 


	return ProjectioMatrix;



}

void FMinimalViewInfo::CalculateProjectionMatrixGivenViewRectangle(const FMinimalViewInfo& ViewInfo, FSceneView& InOutProjectionData)
{
	InOutProjectionData.ProjectionMatrix = ViewInfo.CalculateProjectionMatrix();









}