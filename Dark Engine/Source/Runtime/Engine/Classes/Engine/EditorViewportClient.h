#pragma once
#include "Memory/TUniquePtr.h"
#include "DarkClient.h"
#include "SceneViewport.h"

#include "ViewportClient.h"
#include "Containers/Array.h"



class FEditorWidget
{




};


class FEditorViewportClient : public FViewportClient
{
private:
	TSharedPtr<class UWorld> World;

public:
	FViewport* GameViewport;
	TArray<FEditorWidget> EditorWidgets;

	FSceneViewport* CreateGameViewport();

	void SetViewport(FViewport* InViewport);

	virtual void Draw(FViewport* Viewport) override;

	virtual UWorld* GetWorld();

	FSceneViewport* GetGameViewport();
	const TArray<FEditorWidget>& GetEditorWidgets() const { return EditorWidgets; }

	//virtual void Init();


private:
	void DrawEditor();







};