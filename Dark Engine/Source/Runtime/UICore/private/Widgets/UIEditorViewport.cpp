#include "Widgets/UIEditorViewport.h"
#include "Engine/EditorViewportClient.h"
#include "Engine/SceneViewport.h"




UIEditorViewport::UIEditorViewport():
	UIWidget(TEXT("UIEditorViewport"))
{
	ConstructEditorViewportClient();
	SceneViewport = MakeShareble(new FSceneViewport(EditorViewportClient.get(), this));
	EditorViewportClient->Viewport = SceneViewport.get();
	Size = FIntPoint(-1, -1);
	SceneViewport->SetSize(Size.X, Size.Y);



}




void UIEditorViewport::ConstructEditorViewportClient()
{
	if (EditorViewportClient == nullptr)
	{
		EditorViewportClient = MakeShareble(new FEditorViewportClient(this));
	}


}

void UIEditorViewport::Update(float DeltaTime)
{
	if (Size != LastSize)
	{
		OnResize(Size.X, Size.Y);
		LastSize = Size;
		return;
	}

}

void UIEditorViewport::DrawImGui()
{
	if (ImGui::Begin(-Name, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImVec2 ImSize = ImGui::GetWindowSize();
		Size = FIntPoint(ImSize.x, ImSize.y);

		TRefCountPtr<FRHITexture> Texture = SceneViewport->GetRenderTargetTexture();
		if (Texture)
		{  
			ImGui::SetCursorPos(ImVec2(0, 0));
			ImVec2 SSize = { static_cast<float>(Texture->GetSize().X),  static_cast<float>(Texture->GetSize().Y) };
			ImGui::Image(Texture->GetNativeShaderResourceView(), SSize, ImVec2(0, 0),
				ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
		}
		ImGui::End();
	}
}

void UIEditorViewport::OnResize(int32 NewX, int32 NewY)
{
	SceneViewport->Resize(NewX, NewY);
}


