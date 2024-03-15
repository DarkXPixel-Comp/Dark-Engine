#include "Widgets/UIButton.h"

void UIButton::DrawImGui()
{
	ImGui::SetCursorPos(ImVec2(Position.X, Position.Y));



	/*ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0));*/
	if (ImGui::Button(-Name, ImVec2(Size.X, Size.Y)))
	{
		ButtonDelegate.BroadCast();
	}
}

void UIButton::Update(float DeltaTime)
{
	Rect = FIntRect(Position.X, Position.Y, Position.X + Size.X, Position.Y + Size.Y);


	UIWidget::Update(DeltaTime);
}
