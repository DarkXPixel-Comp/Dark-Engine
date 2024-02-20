#include "Engine/EditorViewportClient.h"
#include "SceneView.h"
#include "SceneInterface.h"



void FEditorViewportClient::Draw(FViewport* InViewport, FCanvas* InCanvas)
{
	GameViewport = InViewport;
	FWorld* World = GetWorld();

	FSceneView SceneView(nullptr, GetScene(), this);

	
	SceneView.CalcSceneView();






}

FSceneInterface* FEditorViewportClient::GetScene() const
{
	FWorld* World = GetWorld();
	if (World)
	{
		return World->Scene;
	}
	return nullptr;

}

void FEditorViewportClient::Tick(float DeltaTime)
{
	



}


FViewportCameraTransform::FViewportCameraTransform() :
	ViewLocation(FVector::ZeroVector),
	ViewRotation(FRotator::ZeroRotator),
	LookAt(FVector::ZeroVector),
	OrthoZoom(1.f)
{
}

void FViewportCameraTransform::SetLocation(const FVector& Position)
{
	ViewLocation = Position;
}

void FViewportCameraTransform::SetRotation(const FRotator& Rotation)
{
	ViewRotation = Rotation;
}

void FViewportCameraTransform::SetLookAt(const FVector& InLookAt)
{
	LookAt = InLookAt;
}

void FViewportCameraTransform::SetOrthoZoom(float InOrthoZoom)
{
	OrthoZoom = InOrthoZoom;
}
