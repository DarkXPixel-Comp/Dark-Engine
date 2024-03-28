#include "Widgets/UIMainMenuBar.h"
#include "Widgets/UIWindow.h"
#include "Math/MathFwd.h"
#include "imgui.h"
#include "Application/UIApplication.h"
#include "imgui_internal.h"
#include "CoreGlobals.h"
#include "imdark_extension.h"
#include "Widgets/UIMenu.h"

UIMainMenuBar::UIMainMenuBar() : UIWidget(TEXT("UIMainMenuBar"))
{
	
}

void UIMainMenuBar::DrawImGui()
{
	//Owner->GetNativeWindow()->bTitleBarHovarered = false;
	const float TitleBarHeight = 58.0f;
	const bool IsMaximized = Owner->GetNativeWindow()->IsMaximized();
	float TitleBarVerticalOffset = IsMaximized ? -6.0f : 0.0f;
	const ImVec2 WindowPadding = ImGui::GetCurrentWindow()->WindowPadding;

	ImGui::SetCursorPos((ImVec2(WindowPadding.x, WindowPadding.y + TitleBarVerticalOffset)));
	const ImVec2 TitleBarMin = ImGui::GetCursorScreenPos();
	const ImVec2 TitleBarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - WindowPadding.y * 2.f,
								ImGui::GetCursorScreenPos().y + TitleBarHeight };
	auto* BgDrawList = ImGui::GetBackgroundDrawList();
	auto* FgDrawList = ImGui::GetForegroundDrawList();

	BgDrawList->AddRectFilled(TitleBarMin, TitleBarMax, ImGui::GetColorU32(ImVec4(0, 0, 0, 1)));

	ImGui::BeginHorizontal("TitleBar", {ImGui::GetWindowWidth() - WindowPadding.y * 2.f, 
		ImGui::GetFrameHeightWithSpacing()});

	static float MoveOffsetX = 0.f;
	static float MoveOffsetY = 0.f;

	const float W = ImGui::GetContentRegionAvail().x;
	const float ButtonAreaWidth = 94;

	ImGui::SetCursorPos(ImVec2(WindowPadding.x , WindowPadding.y + TitleBarVerticalOffset));
	ImGui::InvisibleButton("##titleBarDragZone", ImVec2(W, TitleBarHeight));

	Owner->GetNativeWindow()->bTitleBarHovarered = ImGui::IsItemHovered(ImGuiHoveredFlags_DockHierarchy);


	if (IsMaximized)
	{
		float WindowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
		if (WindowMousePosY >= 0.f && WindowMousePosY <= 5.f)
		{
			Owner->GetNativeWindow()->bTitleBarHovarered = true;
		}
	}


	if (MenuItems.Num())
	{
		ImGui::SuspendLayout();
		ImGui::SetItemAllowOverlap();
		const float LogoHorizontalOffset = 16.f * 2.f + 48.f + WindowPadding.x;
		ImGui::SetCursorPos(ImVec2(LogoHorizontalOffset, 6.f + TitleBarVerticalOffset));
		DrawMenuBar();
		if (ImGui::IsItemHovered())
		{
			Owner->GetNativeWindow()->bTitleBarHovarered = false;
		}


		ImGui::ResumeLayout();

	}
	

	{
		ImVec2 CurrentCursorPos = ImGui::GetCursorPos();
		ImVec2 TextSize = ImGui::CalcTextSize(-Title);
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - TextSize.x * 0.5f, 2.f + WindowPadding.y + 6.f));
		ImGui::Text(-Title);
		ImGui::SetCursorPos(CurrentCursorPos);
	}


	//const ImU32 buttonColN = ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);
	//const ImU32 buttonColH = ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);
	//const ImU32 buttonColP = ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);
	//const float buttonWidth = 14.0f;
	//const float buttonHeight = 14.0f;

	//ImGui::Spring();
	//
	//ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.f);

	//{
	//	const int Icon
	//}



	//ImGui::Spring(-1.f, 18.f);
	ImGui::EndHorizontal();


	ImGui::SetCursorPosY(TitleBarHeight);

}


void UIMainMenuBar::Update(float DeltaTime)
{	
	for (auto& Menu : MenuItems)
	{
		Menu->Update(DeltaTime);

	}
	UIWidget::Update(DeltaTime);
}

void UIMainMenuBar::AddMenu(const TSharedPtr<class UIMenu>& InMenu)
{
	MenuItems.Add(InMenu);
	InMenu->Owner = Owner;
}

void UIMainMenuBar::DrawMenuBar()
{
	const ImRect MenuBarRect = { ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x +
		ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing()} };

	ImGui::BeginGroup();
	if (ImDark::BeginMenubar(MenuBarRect))
	{
		if (ImGui::BeginMenu("Test"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				GIsRequestingExit = true;
			}
			ImGui::EndMenu();
		}

		//for (auto& Menu : MenuItems)
		//{
		//	Menu->DrawImGui();
		//}
	}



	ImDark::EndMenubar();
	ImGui::EndGroup();

}

