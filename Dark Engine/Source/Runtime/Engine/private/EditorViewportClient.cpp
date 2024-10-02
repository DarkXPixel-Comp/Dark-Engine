#include "Engine/EditorViewportClient.h"
#include "SceneView.h"
#include "SceneInterface.h"
#include "RenderGlobals.h"
#include <Widgets/UIWindow.h>
#include "Engine/EditorEngine.h"
#include "Engine/World.h"
#include "Canvas.h"
#include "Math/InverseRotationMatrix.h"	 



struct FEditorViewportViewModifierParams
{
	FEditorViewportClient* ViewportClient = nullptr;

	FMinimalViewInfo ViewInfo;

	friend FEditorViewportClient;
};



FEditorViewportClient::FEditorViewportClient(UIEditorViewport* InEditorViewport):
	EditorViewportWidget(InEditorViewport)
{
	FSceneInterface* Scene = GetScene();

	Engine->AddViewportClient(this);
}


FWorld* FEditorViewportClient::GetWorld() const
{
	FWorld* Result = nullptr;

	Result = GWorld;
	return Result;
}



FSceneView* FEditorViewportClient::CalcSceneView(FSceneView* const View)
{
	FSceneViewInitOptions InitOptions = {};

	FViewportCameraTransform& ViewTransform = GetViewTransform();

	FIntPoint ViewportResolution = Viewport->GetSizeXY();

	FEditorViewportViewModifierParams ViewParams;

	ViewParams.ViewportClient = this;
	ViewParams.ViewInfo.Location = ViewTransform.GetLocation();
	ViewParams.ViewInfo.Rotation = ViewTransform.GetRotation();
	ViewParams.ViewInfo.FOV = 90.f; //<- Temp

	MainViewInfo = ViewParams.ViewInfo;


	InitOptions.ViewOrigin = ViewParams.ViewInfo.Location;


	InitOptions.ViewRotationMatrix = FInverseRotationMatrix(ViewParams.ViewInfo.Rotation) * FMatrix
	(
		FPlane(0, 0, 1, 0),
		FPlane(1, 0, 0, 0),
		FPlane(0, 1, 0, 0),
		FPlane(0, 0, 0, 1)
	);

	FMinimalViewInfo::CalculateProjectionMatrixGivenView(MainViewInfo, Viewport, InitOptions);

								   
	InitOptions.FOV = MainViewInfo.FOV;
	InitOptions.CursorPosition = MousePosition;
	InitOptions.NormalizeCursorPosition = FVector2f((float)MousePosition.X / (float)ViewportResolution.X, (float)MousePosition.Y / (float)ViewportResolution.Y);


	View->Init(InitOptions);


	







	return View;
}


void FEditorViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
	OPTICK_EVENT("EditorViewportClient Draw");
	Viewport = InViewport ? InViewport : Viewport;
	FWorld* World = GetWorld();

	FSceneView* SceneView = new FSceneView(Canvas->GetRenderTarget(), GetScene());
	SceneView = CalcSceneView(SceneView);
	FIntPoint ViewportSize = Viewport->GetSizeXY();
	FIntPoint Offset = FIntPoint(0, 0);
	ViewportSize.X = FMath::Max(1, ViewportSize.X);
	ViewportSize.Y = FMath::Max(1, ViewportSize.Y);
	SceneView->UnscaledRect = FIntRect(Offset, ViewportSize + Offset);


	GetRenderer()->BeginRenderingView(Canvas, SceneView);


	delete SceneView;
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
