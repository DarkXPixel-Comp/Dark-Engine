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



private:
	TSharedPtr<FEditorViewportClient> EditorViewportClient;
	TSharedPtr<FSceneViewport> SceneViewport;
	FIntPoint LastSize;


};
