#include "Widgets/UIWidgetTest.h"
#include "imgui.h"
#include "imgui_internal.h"



void UIWidgetTest::DrawImGui()
{
	ImGuiID MainDockID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);

	//ImGuiID MainDockID = ImGui::DockSpace(0, ImVec2(250, 250));

	ImGui::DockBuilderRemoveNode(MainDockID);

	ImGui::DockBuilderAddNode(MainDockID, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(MainDockID, ImVec2(250, 250));
	
	ImGuiID DockMainID = MainDockID;
	ImGuiID DockIDProp = ImGui::DockBuilderSplitNode(MainDockID, ImGuiDir_Left, 0.2f, NULL, &DockMainID);
	ImGuiID DockIDBottom = ImGui::DockBuilderSplitNode(MainDockID, ImGuiDir_Down, 0.2f, NULL, &DockMainID);
	
	ImGui::DockBuilderDockWindow("Log", DockIDBottom);
	ImGui::DockBuilderDockWindow("Properties", DockIDProp);
	ImGui::DockBuilderDockWindow("Mesh", DockIDProp);
	ImGui::DockBuilderFinish(MainDockID);



}

void UIWidgetTest::Update(float DeltaTime)
{
}