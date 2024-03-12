#include "Widgets/UIButton.h"

void UIButton::DrawImGui()
{
	if (ImGui::Button(-Name, ImVec2(Size.X, Size.Y)))
	{
		ButtonDelegate.BroadCast();
	}
}