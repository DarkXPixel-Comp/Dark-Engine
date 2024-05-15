#include "Widgets/UIEditorViewport.h"
#include "Engine/EditorViewportClient.h"
#include "Engine/SceneViewport.h"
#include "RHICommandList.h"
#include "Misc/Paths.h"
#include <DescriptorHeap.h>
#include "imgui_internal.h"





UIEditorViewport::UIEditorViewport():
	UIWidget(TEXT("UIEditorViewport"))
{
	ConstructEditorViewportClient();
	SceneViewport = MakeShareble(new FSceneViewport(EditorViewportClient.get(), this));
	EditorViewportClient->Viewport = SceneViewport.get();
	Size = FIntPoint(-1, -1);
	SceneViewport->SetSize(Size.X, Size.Y);
	SceneViewport->Resize(1280, 720);



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
	UIViewport::Update(DeltaTime);
	if (SceneViewport->GetRenderTargetTexture() && SceneViewport->GetSizeXY() != SceneViewport->ViewportSize)
	{
		SceneViewport->Resize(SceneViewport->ViewportSize.X, SceneViewport->ViewportSize.Y);
	}
}

void UIEditorViewport::DrawImGui()
{
	if (ImGui::Begin(-Name, IsWidgetClose(), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		auto WindowPadding = Window->GetImGuiContext()->Style.DisplayWindowPadding;
		ImVec2 ImSize = ImGui::GetWindowSize();
		ImSize.y -= WindowPadding.y;
		//SceneViewport->Resize(ImSize.x, ImSize.y);
		SceneViewport->ViewportSize = FIntPoint(ImSize.x, ImSize.y);

		Size = FIntPoint(ImSize.x, ImSize.y);

		FRHITexture* Texture = SceneViewport->GetRenderTargetTexture().Get();
		if (Texture)
		{  
			FRHICommandListImmediate* RHICmdList = &GRHICommandList.GetImmediateCommandList();
			Texture->PrepareShaderResourceView();
			RHICmdList->GetContext().RHIPreparePixelShaderResources();
			ImGui::SetCursorPos(ImVec2(0, WindowPadding.y));
			ImVec2 SSize = { static_cast<float>(Texture->GetSize().X),  static_cast<float>(Texture->GetSize().Y) };
			ImGui::Image(Texture->GetNativeShaderResourceView(), ImSize, ImVec2(0, 0),
				ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
		}


		FIntPoint Size = FIntPoint(ImSize.x, ImSize.y);
		ImVec2 CursorPos = { ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 4, ImGui::GetMousePos().y - ImGui::GetWindowPos().y - WindowPadding.y - 2 };
		if (CursorPos.x < 0 || CursorPos.y < 0 || CursorPos.x > Size.X || CursorPos.y > Size.Y)
		{
			MousePosition = FIntPoint(-1, -1);
		}
		else
		{
			MousePosition = FIntPoint(CursorPos.x, CursorPos.y);
		}

		EditorViewportClient->SetMousePosition(MousePosition);
		//ImGui::GetWindowContentRegionMin();

		ImGui::SetCursorPos(ImVec2(5, 30));
		float Fps = 1 / Delta;

		ImGui::Text("%i", (int32)Fps);

		ImGui::SetCursorPos(ImVec2(5, 45));
		
		ImGui::Text("%ix%i", (int32)Size.X, (int32)Size.Y);

		ImGui::SetCursorPos(ImVec2(5, 60));
		ImGui::Text("%i %i", MousePosition.X, MousePosition.Y);

		ImGui::End();
	}
}

void UIEditorViewport::OnResize(int32 NewX, int32 NewY)
{
	//if (!SceneViewport->GetRenderTargetTexture())
	//{
	//	SceneViewport->Resize(NewX, NewY);
	//}
}

void UIEditorViewport::SetResolution(int32 Width, int32 Height)
{
	if (SceneViewport->GetRenderTargetTexture())
	{
		SceneViewport->Resize(Width, Height);
	}
}


void UIEditorViewport::SetColorBackground(FVector InColor)
{
}

void UIEditorViewport::SetTexturePath(FString Path)
{
	if (SceneViewport->GetRenderTargetTexture())
	{
		SceneViewport->SetTexturePath(Path);
	}
}


