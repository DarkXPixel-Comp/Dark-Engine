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