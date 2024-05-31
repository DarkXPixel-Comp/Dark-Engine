#include "Engine/EditorEngine.h"



TSharedPtr<UIWindow> DEditorEngine::CreateGameWindow()
{
	int32 ResX = 1600;
	int32 ResY = 900;

	const TCHAR* WindowTitle = TEXT("Dark Editor");

	TSharedPtr<UIWindow> Window(new UIWindow());

	Window->ShowWindow();



	return Window;
}

void DEditorEngine::Tick(float DeltaTime)
{
	for (auto& ViewportClient : AllViewportClients)
	{
		ViewportClient->Tick(DeltaTime);
		UpdateViewportClient(ViewportClient);
	}

}

void DEditorEngine::AddViewportClient(FViewportClient* ViewportClient)
{
	AllViewportClients.Add((FEditorViewportClient*)ViewportClient);
}

void DEditorEngine::UpdateViewportClient(FEditorViewportClient* InViewportClient)
{
	if (InViewportClient->IsRealtime())
	{
		InViewportClient->Viewport->Draw();
	}
	CurrentViewportClient = InViewportClient;
}

void DEditorEngine::UpdateViewportClient()
{
	CurrentViewportClient->Viewport->Draw();
}

FWorld* DEditorEngine::NewMap()
{
	FWorld* NewWorld = new FWorld();
	NewWorld->InitializeNewWorld();
	GWorld = NewWorld;
	return NewWorld;
}

void DEditorEngine::Shutdown()
{
	if (GWorld)
	{
		delete GWorld;
		GWorld = nullptr;
	}

}
