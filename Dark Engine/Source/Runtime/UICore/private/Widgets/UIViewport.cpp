#include "Widgets/UIViewport.h"
#include "Engine/EditorViewportClient.h"
#include "Engine/EditorEngine.h"


UIViewport::UIViewport()
{
	FEditorViewportClient* EditorViewport = new FEditorViewportClient();
	ViewportClient = MakeShareble(EditorViewport);
	EditorViewport->SetSize(FIntPoint(800, 600));
	EditorViewport->CreateGameViewport();
	//EditorViewport->SetSize(NativeWindow->GetSize());
}