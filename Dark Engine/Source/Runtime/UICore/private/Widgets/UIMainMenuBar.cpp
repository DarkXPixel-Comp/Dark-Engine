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
	bool bIsMaximized = Owner->GetNativeWindow()->IsMaximized();

	const float TitleBarHeight = 58.f;
	float TitleBarVerticalOffset = bIsMaximized ? -6.f : 0.f;
	const ImVec2 WindowPadding = ImGui::GetCurrentWindow()->WindowPadding;

	ImGui::SetCursorPos(ImVec2(WindowPadding.x, WindowPadding.y + TitleBarVerticalOffset));
	const ImVec2 TitleBarMin = ImGui::GetCursorScreenPos();
	const ImVec2 TitleBarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - WindowPadding.y * 2.0f,
								ImGui::GetCursorScreenPos().y + TitleBarHeight };
	auto* BgDrawList = ImGui::GetBackgroundDrawList();
	auto* FgDrawList = ImGui::GetForegroundDrawList();
	BgDrawList->AddRectFilled(TitleBarMin, TitleBarMax, ImGui::GetColorU32(ImVec4(0, 0, 0, 1)));

	ImGui::BeginHorizontal("TitleBar", { ImGui::GetWindowWidth() - WindowPadding.y * 2.f, ImGui::GetFrameHeightWithSpacing() });

	const float W = ImGui::GetContentRegionAvail().x;
	const float ButtonAreaWidth = 94;

	ImGui::SetCursorPos(ImVec2(WindowPadding.x, WindowPadding.y + TitleBarVerticalOffset));
	ImGui::SetNextItemAllowOverlap();
	ImGui::InvisibleButton("##TitleBarDragZone", ImVec2(W - ButtonAreaWidth, TitleBarHeight));

	Owner->GetNativeWindow()->bTitleBarHovarered = ImGui::IsItemHovered();

	if (bIsMaximized)
	{
		float WindowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
		if (WindowMousePosY >= 0.f && WindowMousePosY <= 5.f)
		{
			Owner->GetNativeWindow()->bTitleBarHovarered = true;
		}
	}

	ImGui::SuspendLayout();



	const float LogoHorizontalOffset = 16.f * 2.f + 48.f + WindowPadding.x;
	ImGui::SetCursorPos(ImVec2(LogoHorizontalOffset, 6.f + TitleBarVerticalOffset));

	const ImRect MenuBarRect = { ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x +
	ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing()} };

	/*if (ImGui::Button("tt"))
	{
		GIsRequestingExit = true;
	}*/

	ImGui::BeginGroup();
	if (ImDark::BeginMenubar(MenuBarRect))
	{
		for (auto& Menu : MenuItems)
		{
			Menu->DrawImGui();
		}



		//if (ImGui::BeginMenu("File"))
		//{
		//	if (ImGui::MenuItem("Exit"))
		//	{
		//		GIsRequestingExit = true;
		//	}
		//	ImGui::EndMenu();
		//}
	}
	ImDark::EndMenubar();
	ImGui::EndGroup();


	/*ImGui::BeginGroup();


	ImDark::EndMenubar();
	ImGui::EndGroup();*/

	if (ImGui::IsItemHovered())
	{
		Owner->GetNativeWindow()->bTitleBarHovarered = false;
	}

	ImGui::ResumeLayout();



	ImVec2 CurrentCursorPos = ImGui::GetCursorPos();
	ImVec2 TextSize = ImGui::CalcTextSize("Dark Engine");
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - TextSize.x * 0.5f, 2.f + WindowPadding.y + 6.f));
	ImGui::Text("Dark Engine");
	ImGui::SetCursorPos(CurrentCursorPos);

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

