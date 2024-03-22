#pragma once
#include "Memory/TUniquePtr.h"
#include "DarkClient.h"
#include "SceneViewport.h"

#include "ViewportClient.h"
#include "Containers/Array.h"
#include "Canvas.h"
#include "Math/MathFwd.h"

#include "Camera/CameraTypes.h"
#include "Widgets/UIWindow.h"
#include "SceneInterface.h"


class UIEditorViewport;

class FEditorWidget
{

};


class FSceneViewport;


struct FViewportCameraTransform
{
public:
	FViewportCameraTransform();
	void SetLocation(const FVector& Position);
	void SetRotation(const FRotator& Rotation);
	void SetLookAt(const FVector& InLookAt);
	void SetOrthoZoom(float InOrthoZoom);

	const FVector& GetLocation() const { return ViewLocation; }
	const FRotator& GetRotation() const { return ViewRotation; }
	const FVector& GetLookAt() const { return LookAt; }
	float GetOrthoZoom() const { return OrthoZoom; }




private:
	FVector ViewLocation;
	FRotator ViewRotation;
	FVector LookAt;
	float OrthoZoom;

};


class FEditorViewportClient : public FViewportClient
{
private:
	TSharedPtr<class FWorld> World;

public:
	FEditorViewportClient(UIEditorViewport* InLevelViewport);	   
	FEditorViewportClient(const FEditorViewportClient&) = delete;
	FEditorViewportClient& const operator=(const FEditorViewportClient&) = delete;

	FSceneViewport* GameViewport;
	FRenderTarget* RenderTarget;
	TArray<FEditorWidget> EditorWidgets;

	void Draw(FViewport* InViewport, FCanvas* InCanvas);

	FSceneViewport* CreateGameViewport();

	FViewport* Viewport;

	void SetViewport(FViewport* InViewport);

	FSceneInterface* GetScene() const;

	const FViewportCameraTransform& GetViewTransform() const { return ViewTransform; }

	virtual FWorld* GetWorld() const;

	FSceneViewport* GetGameViewport();
	const TArray<FEditorWidget>& GetEditorWidgets() const { return EditorWidgets; }

	virtual bool IsRealtime() const { return true; }

	virtual void Tick(float DeltaTime);

	FMinimalViewInfo MainViewInfo;

	float FOV;
	

	//virtual void Init();


private:
	void DrawEditor();
	FViewportCameraTransform ViewTransform;
	UIEditorViewport* EditorViewportWidget;







};