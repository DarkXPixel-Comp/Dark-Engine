#pragma once
#include "Engine/Engine.h"
#include "Widgets/UIWindow.h"
#include "EditorViewportClient.h"	
#include "Containers/Array.h"


class DEditorEngine : public DEdEngine
{
public:
	static TSharedPtr<UIWindow>	CreateGameWindow();

public:
	void Tick(float DeltaTime);
	void Shutdown();

public:
	void AddViewportClient(FViewportClient* ViewportClient) override;

	FWorld* NewMap();


private:
	void UpdateViewportClient(FEditorViewportClient* InViewportClient);
	void UpdateViewportClient();

private:
	TArray<FEditorViewportClient*> AllViewportClients;
	FEditorViewportClient* CurrentViewportClient;





};


