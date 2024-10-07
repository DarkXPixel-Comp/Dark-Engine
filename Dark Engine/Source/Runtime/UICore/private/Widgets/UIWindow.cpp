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
	Widgets.Empty();
	if(UIApplication::Get())
		UIApplication::Get()->GetRenderer()->DestroyViewport(this);
	NativeWindow->Destroy();
	ImGui::DestroyContext(IMGUIContext);
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

	for (auto& Widget : DefferedAddWidgetsWithParent)
	{
		Widget.second->AddChild(Widget.first);
	}

	DeferedAddWidgets.Empty();
	DefferedAddWidgetsWithParent.Empty();
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
	//FRHICommandListImmediate& CmdList = FRHICommandListExecutor::GetImmediateCommandList();
	//if (!Viewport)
	//{
	//	CreateViewport();
	//}
	//CmdList.BeginDrawingViewport(Viewport.get(), nullptr);



}

FIntPoint UIWindow::GetMousePosition() const
{
	return NativeWindow->GetRelativeMousePos();
}
