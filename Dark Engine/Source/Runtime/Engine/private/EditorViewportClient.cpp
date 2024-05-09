#include "Engine/EditorViewportClient.h"
#include "SceneView.h"
#include "SceneInterface.h"
#include "RenderGlobals.h"
#include <Widgets/UIWindow.h>
#include "Engine/EditorEngine.h"
#include "Engine/World.h"
#include "Canvas.h"


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

	FEditorViewportViewModifierParams ViewParams;

	ViewParams.ViewportClient = this;
	ViewParams.ViewInfo.Location = ViewTransform.GetLocation();
	ViewParams.ViewInfo.Rotation = ViewTransform.GetRotation();
	ViewParams.ViewInfo.FOV = 90.f; //<- Temp

	MainViewInfo = ViewParams.ViewInfo;


	InitOptions.ViewOrigin = ViewParams.ViewInfo.Location;


	//InitOptions.ViewRotationMatrix = FInve
	FMinimalViewInfo::CalculateProjectionMatrixGivenView(MainViewInfo, Viewport, InitOptions);

	InitOptions.FOV = MainViewInfo.FOV;
	InitOptions.CursorPosition = FIntPoint(-1, -1);

	View->Init(InitOptions);
	







	return View;
}


void FEditorViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
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
