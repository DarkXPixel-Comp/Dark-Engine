#include "Application/UIApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Math/Vector2D.h"
#include "imgui.h"
#include "imgui_spectrum.h"
#include "imgui_internal.h"

#include "d3d12.h"
#include "DescriptorHeap.h"
#include <dxgi.h>
//#include <d3dx12.h>
#include "Timer/GameTimer.h"
#include "Misc/Paths.h"
#include "Engine/Engine.h"



TSharedPtr<UIApplication> UIApplication::CurrentApplication = nullptr;
TSharedPtr<FGenericApplication> UIApplication::PlatformApplication = nullptr;



void embraceTheDarkness();




TSharedPtr<UIApplication> UIApplication::Create(const TSharedPtr<FGenericApplication>& InPlatformApplictation)
{
	PlatformApplication = InPlatformApplictation;
	CurrentApplication = MakeShareble(new UIApplication());

	PlatformApplication->SetMessageHandler(CurrentApplication);


	FGameTimer::Start();


	return CurrentApplication;
}						  
						
TSharedPtr<UIApplication> UIApplication::Create()
{
	return Create(MakeShareble(FPlatformApplicationMisc::CreateApplication()));
}



TSharedPtr<FGenericWindow> UIApplication::MakeWindow(TSharedPtr<UIWindow> InUIWindow)
{
	TSharedPtr<FGenericWindowDefinition> Definition = MakeShareble(new FGenericWindowDefinition());

	const FVector2f Size = InUIWindow->GetInitSizeInScreen();
	Definition->WidthOnScreen = Size.X;
	Definition->HeightOnScreen = Size.Y;



	const FVector2f Position = InUIWindow->GetInitPositionInScreen();
	Definition->XPositionOnScreen = Position.X;
	Definition->YPositionOnScreen = Position.Y;
	Definition->bHasWindowBorder = InUIWindow->HasWindowBorder();

	Definition->Title = InUIWindow->GetTitle();

	TSharedPtr<FGenericWindow> Window = PlatformApplication->MakeWindow();

	InUIWindow->SetNativeWindow(Window);

#ifdef IMGUI
	InUIWindow->SetImGuiContext(ImGui::CreateContext());
	InUIWindow->GetImGuiContext()->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable |
		ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_NavEnableKeyboard;
	InUIWindow->GetImGuiContext()->IO.IniFilename = "../Config/UIImgui.ini";
//	InUIWindow->GetImGuiContext()->IO.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui::Spectrum::LoadFont();
	embraceTheDarkness();
#endif

	PlatformApplication->InitializeWindow(Window, Definition);


	return Window;
}




bool UIApplication::OnWindowDraw(const TSharedPtr<FGenericWindow>& Window)
{
	//FGameTimer::Tick();
	////Engine->Tick(1);
	//DrawWindows();
	return true;
}


bool UIApplication::OnSizeChanged(const TSharedPtr<FGenericWindow>& InWindow, const int32 Width, const int32 Height, bool bWasMinimized)
{
	UIWindow* Window = FindUIWindowByNative(InWindow);
	Renderer->Resize(Window, Width, Height, bWasMinimized);
	Window->SetSizeViewport(FVector2f(Width, Height));

	DrawWindows();
	return true;
}

bool UIApplication::OnMouseDown(const TSharedPtr<FGenericWindow>& InWindow,
	const EMouseButtons::Type Button, const FVector2D CursorPos)
{
	UIWindow* Window = FindUIWindowByNative(InWindow);
	check(Window);

	PlatformApplication->SetCapture(InWindow);

	OnMouseDownDelegate.BroadCast(Window, Button, CursorPos);


	return true;
}

bool UIApplication::OnMouseUp(const EMouseButtons::Type Button, const FVector2D CursorPos)
{
	OnMouseUpDelegate.BroadCast(Button, CursorPos);
	PlatformApplication->SetCapture(nullptr);
	return true;
}

bool UIApplication::OnMovedWindow(const TSharedPtr<FGenericWindow>& Window, FIntPoint NewPosition)
{
	//FGameTimer::Tick();
	//TickAndDrawWidgets(FGameTimer::DeltaTime());
	return true;
}

bool UIApplication::OnMovingWindow(const TSharedPtr<FGenericWindow>& Window, FIntRect NewRect)
{
	DrawWindows();
	return true;
}

void UIApplication::InitializeRenderer(TSharedPtr<FUIRenderer> InRenderer)
{
	check(InRenderer);
	Renderer = InRenderer;
	Renderer->Initialize();
}

void UIApplication::Tick(float DeltaTime)
{
	//float DeltaTime = FGameTimer::DeltaTime();

	TickPlatform(DeltaTime);


	TickAndDrawWidgets(DeltaTime);
}

void UIApplication::TickPlatform(float DeltaTime)
{
	if (UIWindows.Num() > 0)
	{
		PlatformApplication->PumpMessages();
	}
	PlatformApplication->Tick(DeltaTime);
}





void UIApplication::TickAndDrawWidgets(float DeltaTime)
{
	DrawWindows();

	for (auto& Window : UIWindows)
	{
		Window->Update(DeltaTime);
	}
}

void UIApplication::DrawWindows()
{	
	check(Renderer);

	
	Renderer->DrawWindows(UIWindows);
}


TSharedPtr<UIWindow> UIApplication::AddWindow(TSharedPtr<UIWindow> InWindow)
{
	TSharedPtr<FGenericWindow> NewWindow = MakeWindow(InWindow);
	UIWindows.Add(InWindow);
	return InWindow;
}



void UIApplication::RegisterGameViewport(TSharedPtr<UIViewport> InViewport)
{
	GameViewport = InViewport;
	AllGameViewports.Add(InViewport);
}

void embraceTheDarkness()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	//colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 2.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.TabBorderSize = 1;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}


FIntPoint UIApplication::GetMousePosition() const
{
	return PlatformApplication->GetMousePos();
}


UIWindow* const UIApplication::FindUIWindowByNative(TSharedPtr<FGenericWindow> InWindow)
{
	for (auto& Window : UIWindows)
	{
		if (Window->GetNativeWindow() == InWindow)
		{
			return Window.get();
		}
	}
	return nullptr;
}
