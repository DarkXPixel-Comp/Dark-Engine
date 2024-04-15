#include "Widgets/UIWindow.h"
#include "Engine/EditorEngine.h"
#include "RHICommandList.h"
#include "Application/UIApplication.h"


void UIWindow::SetNativeWindow(TSharedPtr<FGenericWindow> InNativeWindow)
{
	NativeWindow = InNativeWindow;
}

UIWindow::~UIWindow()
{
	NativeWindow->Destroy();
#ifdef IMGUI
	ImGui::DestroyContext();
#endif
}


void UIWindow::ShowWindow()
{
	if (NativeWindow)
	{
		UIApplication::Get()->GetRenderer()->CreateViewport(this);
		NativeWindow->Show();
	}
}



void UIWindow::Update(float DeltaTime)
{
	if(bTitleChanged)
	{
		NativeWindow->SetTitle(Title);
		bTitleChanged = false;
	}

	for (auto& Widget : DeferedAddWidgets)
	{
		Widgets.Add(Widget);
		Widget->InitWindow(this);
	}
	for (auto& Widget : DeferedRemoveWidgets)
	{
		Widgets.Remove(Widget);
	}
	DeferedAddWidgets.Empty();
	DeferedRemoveWidgets.Empty();
	

	for (auto& Widget : Widgets)
	{
		Widget->Update(DeltaTime);
	}
}



TSharedPtr<FViewportClient> UIWindow::CreateViewportClient()
{
	/*FEditorViewportClient* EditorViewport = new FEditorViewportClient();
	ViewportClient = MakeShareble(EditorViewport);
	EditorViewport->SetSize(NativeWindow->GetSize());*/
	

	return nullptr;
}



void UIWindow::CreateViewport()
{
	Viewport = MakeShareble(new FRHIViewport());

}



void UIWindow::DrawWindow()
{	
	FRHICommandListImmediate& CmdList = FRHICommandListExecutor::GetImmediateCommandList();
	if (!Viewport)
	{
		CreateViewport();
	}
	CmdList.BeginDrawingViewport(Viewport.get(), nullptr);



}

FIntPoint UIWindow::GetMousePosition() const
{
	return NativeWindow->GetRelativeMousePos();
}
