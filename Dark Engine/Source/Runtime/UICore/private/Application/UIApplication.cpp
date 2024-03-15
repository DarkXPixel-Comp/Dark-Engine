#include "Application/UIApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Math/Vector2D.h"
#include "imgui.h"
#include "imgui_spectrum.h"
#include "imgui_internal.h"

#include "d3d12.h"
#include "DescriptorHeap.h"
#include <dxgi.h>
#include <Runtime/D3D12RHI/public/d3dx12.h>
#include "Timer/GameTimer.h"
#include "Misc/Paths.h"



TSharedPtr<UIApplication> UIApplication::CurrentApplication = nullptr;
TSharedPtr<FGenericApplication> UIApplication::PlatformApplication = nullptr;







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


bool UIApplication::OnSizeChanged(const TSharedPtr<FGenericWindow>& InWindow, const int32 Width, const int32 Height, bool bWasMinimized)
{
	UIWindow* Window = FindUIWindowByNative(InWindow);
	Renderer->Resize(Window, Width, Height, bWasMinimized);
	Window->SetSizeViewport(FVector2f(Width, Height));


	return true;
}

void UIApplication::InitializeRenderer(TSharedPtr<FUIRenderer> InRenderer)
{
	check(InRenderer);
	Renderer = InRenderer;
	Renderer->Initialize();


}

void UIApplication::Tick()
{
	float DeltaTime = FGameTimer::DeltaTime();

	TickPlatform(DeltaTime);

	FGameTimer::Tick();

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




FIntPoint UIApplication::GetMousePosition() const
{

	return PlatformApplication->GetMousePos();
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
	InUIWindow->GetImGuiContext()->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

	//SET IMGUI STYLE
	{
		//ImGui::GetIO().Fonts->AddFontFromFileTTF((FPaths::EngineContentDir() + "Fonts/Roboto-Regular.ttf").GetStr(),
		//	14);	 //temp

		//ImGui::StyleS
		ImGui::Spectrum::LoadFont();
		ImGui::Spectrum::StyleColorsSpectrum();

		//auto &style = ImGui::GetStyle();
		//auto& colors = style.Colors;
	

		//style.WindowPadding = ImVec2(9, 5);
		//style.WindowRounding = 10;
		//style.FramePadding = ImVec2(5, 3);
		//style.FrameRounding = 6.0; 
		//style.ItemSpacing = ImVec2(9.0, 3.0);
		//style.ItemInnerSpacing = ImVec2(9.0, 3.0);
		//style.IndentSpacing = 21;
		//style.ScrollbarSize = 6.0;
		//style.ScrollbarRounding = 13;
		//style.GrabMinSize = 17.0;
		//style.GrabRounding = 16.0;
		//style.WindowTitleAlign = ImVec2(0.5, 0.5);
		//style.ButtonTextAlign = ImVec2(0.5, 0.5);


		//colors[ImGuiCol_Text] = ImVec4(0.90, 0.90, 0.90, 1.00);
		//colors[ImGuiCol_TextDisabled] = ImVec4(1.00, 1.00, 1.00, 1.00);
		//colors[ImGuiCol_WindowBg] = ImVec4(0.00, 0.00, 0.00, 1.00);
		//colors[ImGuiCol_ChildBg] = ImVec4(0.00, 0.00, 0.00, 1.00);
		//colors[ImGuiCol_PopupBg] = ImVec4(0.00, 0.00, 0.00, 1.00);
		//colors[ImGuiCol_Border] = ImVec4(0.82, 0.77, 0.78, 1.00);
		//colors[ImGuiCol_BorderShadow] = ImVec4(0.35, 0.35, 0.35, 0.66);
		//colors[ImGuiCol_FrameBg] = ImVec4(1.00, 1.00, 1.00, 0.28);
		//colors[ImGuiCol_FrameBgHovered] = ImVec4(0.68, 0.68, 0.68, 0.67);
		//colors[ImGuiCol_FrameBgActive] = ImVec4(0.79, 0.73, 0.73, 0.62);
		//colors[ImGuiCol_TitleBg] = ImVec4(0.00, 0.00, 0.00, 1.00);
		//colors[ImGuiCol_TitleBgActive] = ImVec4(0.46, 0.46, 0.46, 1.00);
		//colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00, 0.00, 0.00, 1.00);
		//colors[ImGuiCol_MenuBarBg] = ImVec4(0.00, 0.00, 0.00, 0.80);
		//colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00, 0.00, 0.00, 0.60);
		//colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00, 1.00, 1.00, 0.87);
		//colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00, 1.00, 1.00, 0.79);
		//colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80, 0.50, 0.50, 0.40);
		////colors[ImGuiCol_ComboBg] = ImVec4(0.24, 0.24, 0.24, 0.99);
		//colors[ImGuiCol_CheckMark] = ImVec4(0.99, 0.99, 0.99, 0.52);
		//colors[ImGuiCol_SliderGrab] = ImVec4(1.00, 1.00, 1.00, 0.42);
		//colors[ImGuiCol_SliderGrabActive] = ImVec4(0.76, 0.76, 0.76, 1.00);
		//colors[ImGuiCol_Button] = ImVec4(0.51, 0.51, 0.51, 0.60);
		//colors[ImGuiCol_ButtonHovered] = ImVec4(0.68, 0.68, 0.68, 1.00);
		//colors[ImGuiCol_ButtonActive] = ImVec4(0.67, 0.67, 0.67, 1.00);
		//colors[ImGuiCol_Header] = ImVec4(0.72, 0.72, 0.72, 0.54);
		//colors[ImGuiCol_HeaderHovered] = ImVec4(0.92, 0.92, 0.95, 0.77);
		//colors[ImGuiCol_HeaderActive] = ImVec4(0.82, 0.82, 0.82, 0.80);
		//colors[ImGuiCol_Separator] = ImVec4(0.73, 0.73, 0.73, 1.00);
		//colors[ImGuiCol_SeparatorHovered] = ImVec4(0.81, 0.81, 0.81, 1.00);
		//colors[ImGuiCol_SeparatorActive] = ImVec4(0.74, 0.74, 0.74, 1.00);
		//colors[ImGuiCol_ResizeGrip] = ImVec4(0.80, 0.80, 0.80, 0.30);
		//colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.95, 0.95, 0.95, 0.60);
		//colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00, 1.00, 1.00, 0.90);
		////colors[ImGuiCol_CloseButton] = ImVec4(0.45, 0.45, 0.45, 0.50);
		////colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70, 0.70, 0.90, 0.60);
		////colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70, 0.70, 0.70, 1.00);
		//colors[ImGuiCol_PlotLines] = ImVec4(1.00, 1.00, 1.00, 1.00);
		//colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00, 1.00, 1.00, 1.00);
		//colors[ImGuiCol_PlotHistogram] = ImVec4(1.00, 1.00, 1.00, 1.00);
		//colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00, 1.00, 1.00, 1.00);
		//colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00, 1.00, 1.00, 0.35);
		////colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.88, 0.88, 0.88, 0.35);

	}

	//Window->InitImGui();
#endif

	PlatformApplication->InitializeWindow(Window, Definition);


	return Window;
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
