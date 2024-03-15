#include "Widgets/UIDock.h"
#include "imgui.h"

void UIDock::DrawImGui()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), 0);


	ForEachChild([](UIWidget* Widget)
		{
			Widget->DrawImGui();
		});

}

void UIDock::Update(float DeltaTime)
{
	
	UIWidget::Update(DeltaTime);
}