#pragma once
#include "Widgets/UIWidget.h"
#include "Widgets/UIViewport.h"

class FEditorViewportClient;
class FSceneViewport;


class UIEditorViewport : public virtual UIViewport
{
public:
	UIEditorViewport();
	virtual ~UIEditorViewport() = default;
	void ConstructEditorViewportClient();

	void Update(float DeltaTime);
	void DrawImGui();
	void OnResize(int32 NewX, int32 NewY);
	void SetResolution(int32 Width, int32 Height);
	FIntPoint GetResoulution() const { return SceneViewport->GetSizeXY(); }
	void SetColorBackground(FVector InColor);
	void SetTexturePath(FString Path);

	const TSharedPtr<FSceneViewport>& GetSceneViewport() const { return SceneViewport; }

	
private:
	TSharedPtr<FEditorViewportClient> EditorViewportClient;
	TSharedPtr<FSceneViewport> SceneViewport;
	FIntPoint LastSize;
	bool SkipImage = false;


};
