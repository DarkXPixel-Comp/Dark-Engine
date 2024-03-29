#pragma once
#include "Widgets/UIWidget.h"
#include "Widgets/UIViewport.h"

class FEditorViewportClient;
class FSceneViewport;


class UIEditorViewport : public virtual UIViewport
{
public:
	UIEditorViewport();
	void ConstructEditorViewportClient();

	void Update(float DeltaTime);
	void DrawImGui();
	void OnResize(int32 NewX, int32 NewY);
	void SetResolution(int32 Width, int32 Height);
	void SetColorBackground(FVector InColor);
	void SetTexturePath(FString Path);
	
private:
	TSharedPtr<FEditorViewportClient> EditorViewportClient;
	TSharedPtr<FSceneViewport> SceneViewport;
	FIntPoint LastSize;
	bool SkipImage = false;


};
