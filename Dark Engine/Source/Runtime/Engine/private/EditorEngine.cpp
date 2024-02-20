#include "Engine/EditorEngine.h"



TSharedPtr<UIWindow> DEditorEngine::CreateGameWindow()
{
	int32 ResX = 1600;
	int32 ResY = 900;

	const TCHAR* WindowTitle = TEXT("Dark Editor");

	TSharedPtr<UIWindow> Window(new UIWindow());

	Window->ShowWindow();




	return TSharedPtr<UIWindow>();
}

void DEditorEngine::Tick(float DeltaTime)
{
	for (auto& ViewportClient : AllViewportClients)
	{
		ViewportClient->Tick(DeltaTime);
	}

	





}

void DEditorEngine::UpdateViewportClient(FEditorViewportClient* InViewportClient)
{
	if (InViewportClient->IsRealtime())
	{
		InViewportClient->GameViewport->Draw();
	}







}
