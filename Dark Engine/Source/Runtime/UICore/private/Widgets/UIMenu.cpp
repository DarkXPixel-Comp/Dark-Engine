#include "Widgets/UIMenu.h"

void UIMenu::DrawImGui()
{
	//ImGui::SetNextWindowSize(ImVec2(Size.X, Size.Y));
	//ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_MenuBar);
	
	if (ImGui::BeginMenu(Name.ToString().c_str()))
	{
		ImGui::SetWindowSize(Name.ToString().c_str(), ImVec2(Size.X, Size.Y));
		MenuDelegate.BroadCast();

		ForEachChild([](UIWidget* Widget)
			{
				Widget->DrawImGui();
			});


		ImGui::EndMenu();
	}





}

void UIMenu::Update(float DeltaTime)
{
	Size = FIntPoint(ImGui::CalcTextSize(Name.GetStr()).x, ImGui::CalcTextSize(Name.GetStr()).y);
	Position = 0;

	Rect = FIntRect(Position, Size);

	UIWidget::Update(DeltaTime);
}

void UIMenuItem::DrawImGui()
{
	ImGui::MenuItem(-Name, NULL, &bPressed);
}

void UIMenuItem::Update(float DeltaTime)
{
	if (bPressed)
	{
		MenuItemDelegate.BroadCast();
		bPressed = false;
	}
}
