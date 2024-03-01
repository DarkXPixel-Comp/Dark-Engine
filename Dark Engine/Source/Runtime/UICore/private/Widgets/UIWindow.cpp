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