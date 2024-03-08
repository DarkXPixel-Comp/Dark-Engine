#include "Engine/EditorViewportClient.h"
#include "SceneView.h"
#include "SceneInterface.h"
#include "RenderGlobals.h"
#include <Widgets/UIWindow.h>
#include "Engine/EditorEngine.h"




FEditorViewportClient::FEditorViewportClient()
{
	Engine->AddViewportClient(this);
}

void FEditorViewportClient::Draw(FViewport* InViewport, FCanvas* InCanvas)
{
	GameViewport = (FSceneViewport*)InViewport;
	FWorld* World = GetWorld();

	FSceneView SceneView(GameViewport, GetScene(), this);

	
	SceneView.CalcSceneView();

	GetRenderer()->BeginRenderingView(InCanvas, &SceneView);

	GetRenderer()->RenderUI(RenderTarget);


}



FSceneViewport* FEditorViewportClient::CreateGameViewport()
{
	//GameViewport = new FSceneViewport();
	//GameViewport->SetSize()
	return GameViewport;
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
