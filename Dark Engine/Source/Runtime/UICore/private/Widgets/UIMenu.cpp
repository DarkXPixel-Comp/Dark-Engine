#include "Widgets/UIMenu.h"

void UIMenu::DrawImGui()
{
	//ImGui::SetNextWindowSize(ImVec2(Size.X, Size.Y));
	//ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_MenuBar);
	
	if (ImGui::BeginMenu(Name.ToString().c_str()))
	{  
		//ImGui::SetWindowSize(Name.ToString().c_str(), ImVec2(Size.X, Size.Y));
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
	UIWidget::Update(DeltaTime);
}

void UIMenuItem::DrawImGui()
{
	if (ImGui::MenuItem(-Name, NULL, &bPressed))
	{

	}
}

void UIMenuItem::Update(float DeltaTime)
{
	if (ChildWidgets.Num() == 1 && ChildWidgets[0]->bHaveCloseButton && (ChildWidgets[0]->IsOpen() != bPressed))
	{
		bPressed = ChildWidgets[0]->IsOpen();
		bLastPressed = bPressed;
	}
	if (bPressed != bLastPressed)
	{
		MenuItemDelegate.BroadCast(bPressed);
		bLastPressed = bPressed;
	}
}
