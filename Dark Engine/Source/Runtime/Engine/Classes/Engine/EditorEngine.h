#pragma once
#include "Engine/Engine.h"
#include "Widgets/UIWindow.h"
#include "EditorViewportClient.h"	
#include "Containers/Array.h"


class DEditorEngine : public DEngine
{
public:
	static TSharedPtr<UIWindow>	CreateGameWindow();

public:
	void Tick(float DeltaTime);


private:
	void UpdateViewportClient(FEditorViewportClient* InViewportClient);


private:
	TArray<FEditorViewportClient*> AllViewportClients;
	FEditorViewportClient* CurrentViewportClient;





};