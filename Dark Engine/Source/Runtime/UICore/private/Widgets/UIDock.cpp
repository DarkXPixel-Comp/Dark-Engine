#include "Widgets/UIDock.h"
#include "imgui.h"
#include "Widgets/UIWindow.h"
#include "Widgets/UIMainMenuBar.h"
#include <imdark_extension.h>
#include "CoreGlobals.h"

void UIDock::DrawImGui()
{
	Window->GetNativeWindow()->bTitleBarHovarered = false;

	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDocking;

	ImGuiViewport* Viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(Viewport->Pos);
	ImGui::SetNextWindowSize(Viewport->Size);
	ImGui::SetNextWindowViewport(Viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	const bool bIsMaximized = Window->GetNativeWindow()->IsMaximized();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, bIsMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);


	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("DockSpaceWindow", nullptr, WindowFlags);
	ImGui::PopStyleColor(); // MenuBarBg
	ImGui::PopStyleVar(2);
	ImGui::PopStyleVar(2);


	if (MainMenuBar)
	{
		MainMenuBar->DrawImGui();
	}


	ImGuiIO& IO = ImGui::GetIO();
	ImGuiStyle& Style = ImGui::GetStyle();
	float MinWinSizeX = Style.WindowMinSize.x;

	Style.WindowMinSize.x = 370.f;
	ImGui::DockSpace(ImGui::GetID("MyDockspace"));
	//ImGui::DockSpaceOverViewport(Viewport);
	Style.WindowMinSize.x = MinWinSizeX;


	ForEachChild([](UIWidget* Widget)
			{
				Widget->DrawImGui();
			});

	ImGui::End();
										 
}

void UIDock::Update(float DeltaTime)
{
	if (MainMenuBar)
	{
		MainMenuBar->Update(DeltaTime);
	}
	UIWidget::Update(DeltaTime);
}

void UIDock::SetMainMenuBar(TSharedPtr<class UIMainMenuBar> InMenuBar)
{
	MainMenuBar = InMenuBar; MainMenuBar->Window = Window;
}
