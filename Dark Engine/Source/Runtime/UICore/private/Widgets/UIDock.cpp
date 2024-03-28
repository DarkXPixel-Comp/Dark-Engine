#include "Widgets/UIDock.h"
#include "imgui.h"
#include "Widgets/UIWindow.h"
#include "Widgets/UIMainMenuBar.h"
#include <imdark_extension.h>
#include "CoreGlobals.h"

void UIDock::DrawImGui()
{
	Owner->GetNativeWindow()->bTitleBarHovarered = false;

	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDocking;

	ImGuiViewport* Viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(Viewport->Pos);
	ImGui::SetNextWindowSize(Viewport->Size);
	ImGui::SetNextWindowViewport(Viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	const bool bIsMaximized = Owner->GetNativeWindow()->IsMaximized();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, bIsMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);


	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("DockSpaceWindow", nullptr, WindowFlags);
	ImGui::PopStyleColor(); // MenuBarBg
	ImGui::PopStyleVar(2);
	ImGui::PopStyleVar(2);


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
	const float ButtonAreaWidth= 94;
	
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
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				GIsRequestingExit = true;
			}
			ImGui::EndMenu();
		}
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

	ImGuiIO& IO = ImGui::GetIO();
	ImGuiStyle& Style = ImGui::GetStyle();
	float MinWinSizeX = Style.WindowMinSize.x;

	Style.WindowMinSize.x = 370.f;
	ImGui::DockSpace(ImGui::GetID("MyDockspace"));
	//ImGui::DockSpaceOverViewport(Viewport);
	Style.WindowMinSize.x = MinWinSizeX;

	ImGui::Begin("Hello");
	ImGui::Button("Button");
	ImGui::End();	  

	ImGui::ShowDemoWindow();


	/*ForEachChild([](UIWidget* Widget)
			{
				Widget->DrawImGui();
			});*/

	ImGui::End();
										 






	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), 0);

	//ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDocking;

	//ImGuiViewport* Viewport = ImGui::GetMainViewport();

	//ImGui::SetNextWindowPos(Viewport->Pos);
	//ImGui::SetNextWindowSize(Viewport->Size);
	//ImGui::SetNextWindowViewport(Viewport->ID);

	//WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
	//	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;	 
	//
	//const bool IsMaximized = Owner->GetNativeWindow()->IsMaximized();
	//
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, IsMaximized ? ImVec2(6.f, 6.f) : ImVec2(1.f, 1.f));

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

	//ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

	//ImGui::Begin("DockSpaceWindow", nullptr, WindowFlags);

	//ImGui::PopStyleColor(); // MenuBarBg
	//ImGui::PopStyleVar(2);

	////ImGui::PopStyleVar(2);


	//if (MainMenuBar)
	//{
	//	MainMenuBar->DrawImGui();
	//}

	//ImGuiIO& IO = ImGui::GetIO();
	//ImGuiStyle& Style = ImGui::GetStyle();
	//float MinWinSizeX = Style.WindowMinSize.x;
	//Style.WindowMinSize.x = 370.f;
	//ImGui::DockSpace(ImGui::GetID("MyDockspace"));
	//Style.WindowMinSize.x = MinWinSizeX;


	//ForEachChild([](UIWidget* Widget)
	//	{
	//		Widget->DrawImGui();
	//	});

	//ImGui::End();

}

void UIDock::Update(float DeltaTime)
{
	/*if (MainMenuBar)
	{
		MainMenuBar->Update(DeltaTime);
	}
	UIWidget::Update(DeltaTime);*/
}

void UIDock::SetMainMenuBar(TSharedPtr<class UIMainMenuBar> InMenuBar)
{
	MainMenuBar = InMenuBar; MainMenuBar->Owner = Owner;
}
