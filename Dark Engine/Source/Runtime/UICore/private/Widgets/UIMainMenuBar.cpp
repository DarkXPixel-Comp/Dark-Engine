#include "Widgets/UIMainMenuBar.h"
#include "Widgets/UIWindow.h"
#include "Math/MathFwd.h"
#include "imgui.h"

void UIMainMenuBar::DrawImGui()
{
	TArray<ImVec4> Rects;

	if (ImGui::BeginMainMenuBar())
	{
		ForEachChild([](UIWidget* Widget)
			{
				Widget->DrawImGui();
			});

		//ImGui::IsWindowFocused();
		ImGui::EndMainMenuBar();
	}



}

void UIMainMenuBar::Update(float DeltaTime)
{
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		check(true);
	}
	
	FIntRect TitleBarRect(0, 0, Owner->GetViewportSize().X, Size.Y);
	Rect = TitleBarRect;


	if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) && bMouseLeftIsDown)
	{
		bMouseLeftIsDown = false;
	}


	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		if (TitleBarRect.IsPointInRect(Owner->GetMousePosition())
			&& !IsRects() && !bMouseLeftIsDown)
		{
			ReleaseCapture();
			//SetCapture((HWND)Owner->GetNativeWindow()->GetOSWindowHandle());
			SendMessage((HWND)Owner->GetNativeWindow()->GetOSWindowHandle(), 0xA1, 0x2, 0);
		}
		else
		{
			bMouseLeftIsDown = true;
		}
	}

	/*if (ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
		TitleBarRect.IsPointInRect(Owner->GetMousePosition())
		&& !IsRects())
	{
		ReleaseCapture();
		SendMessage((HWND)Owner->GetNativeWindow()->GetOSWindowHandle(), 0xA1, 0x2, 0);

	}*/

	UIWidget::Update(DeltaTime);
}

bool UIMainMenuBar::IsRects() const
{
	for (auto Child : ChildWidgets)
	{
		FIntRect Rect = Child->GetRect();

		if (Rect.IsPointInRect(Owner->GetMousePosition()))
		{
			return true;
		}
	}
	return false;
}
