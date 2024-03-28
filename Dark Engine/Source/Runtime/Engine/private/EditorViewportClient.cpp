#include "Engine/EditorViewportClient.h"
#include "SceneView.h"
#include "SceneInterface.h"
#include "RenderGlobals.h"
#include <Widgets/UIWindow.h>
#include "Engine/EditorEngine.h"
#include "Engine/World.h"
#include "Canvas.h"




FEditorViewportClient::FEditorViewportClient(UIEditorViewport* InEditorViewport):
	EditorViewportWidget(InEditorViewport)
{
	FSceneInterface* Scene = GetScene();

	Engine->AddViewportClient(this);
}


FWorld* FEditorViewportClient::GetWorld() const
{
	FWorld* Result = nullptr;

	Result = &GWorld;
	return Result;
}

void FEditorViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
	Viewport = InViewport ? InViewport : Viewport;
	FWorld* World = GetWorld();

	FSceneView SceneView(Canvas->GetRenderTarget(), GetScene());

	
	SceneView.CalcSceneView();

	//GetRenderer()->BeginRenderingView(InCanvas, &SceneView);


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
