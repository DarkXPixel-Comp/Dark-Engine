#include "Rendering/UIRHIRenderer.h"
#include "Math/MathFwd.h"
#include <DynamicRHI.h>
#include "RHICommandList.h"
#include <CoreGlobals.h>
#include "Console/GlobalInputOutConsole.h"


void FUIRHIRenderer::Initialize()
{
	GGlobalConsole.RegisterConsoleVariableRef(TEXT("r.D3D12.Vsync"), Vsync, TEXT("Vertical sync"))->SetOnChangedCallback([](IConsoleVariable* Var)
		{
			if (Var->AsVariable()->GetInt() > 4)
			{
				Var->AsVariable()->Set(4);
			}
		});

}


void FUIRHIRenderer::CreateViewport(UIWindow* InWindow)
{
	const FVector2f	WindowSize = InWindow->GetInitSizeInScreen();

	FViewportInfo* NewInfo = new FViewportInfo();

	NewInfo->WindowHandle = InWindow->GetNativeWindow()->GetOSWindowHandle();
	NewInfo->Width = WindowSize.X;
	NewInfo->Height = WindowSize.Y;
	NewInfo->bFullscreen = IsViewportFullscreen(*InWindow);
	NewInfo->ViewportRHI = CreateRHIViewport(NewInfo->WindowHandle, WindowSize.X, WindowSize.Y, NewInfo->bFullscreen);

	WindowToViewportInfo.emplace(InWindow, NewInfo);




}

void FUIRHIRenderer::Resize(UIWindow* InWindow, const int32 Width, const int32 Height, const bool bWasMinimized)
{																	 
	FViewportInfo* Info = WindowToViewportInfo.find(InWindow)->second;
	Info->bFullscreen = IsViewportFullscreen(*InWindow);
	Info->Width = Width;
	Info->Height = Height;
	Info->ViewportRHI->Resize(Width, Height, bWasMinimized);
}

void FUIRHIRenderer::DrawWindows(const TArray<TSharedPtr<UIWindow>>& InWindows)
{
	FRHICommandListImmediate* RHICmdList = &GRHICommandList.GetImmediateCommandList();

	for (auto& Window : InWindows)
	{
		if (Window)
		{
			const FVector2f ViewportSize = Window->GetViewportSize();

			if (ViewportSize.X > 0 && ViewportSize.Y > 0)
			{
				FViewportInfo* ViewInfo = nullptr;


				FViewportInfo* FoundViewInfo = WindowToViewportInfo.find(Window.get())->second;
				if (FoundViewInfo)
				{
					ViewInfo = FoundViewInfo;
				}
				else
				{
					continue;
				}

				bool bVsync = 1;


				FRHITexture* BackBuffer = RHIGetViewportBackBuffer(ViewInfo->ViewportRHI.get());
				if (!BackBuffer)
				{
					return;
				}
				RHICmdList->BeginDrawingViewport(ViewInfo->ViewportRHI.get(), nullptr);
				RHICmdList->BeginFrame();
#ifdef IMGUI
				RHICmdList->BeginImGui();
				Window->GetNativeWindow()->BeginImGui();

				FRHIRenderPassInfo RPInfo(BackBuffer);
				RHICmdList->BeginRenderPass(RPInfo);
				ImGui::NewFrame();
#endif
				for (auto& Widget : Window->GetWidgets())
				{
					Widget->DrawImGui();
				}
				ImGui::Render();

#ifdef IMGUI
				RHICmdList->EndImGui();
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}
#endif

				RHICmdList->EndRenderPass(RPInfo);
				//RHICmdList->EndFrame();
				RHICmdList->EndDrawingViewport(ViewInfo->ViewportRHI.get(), true, Vsync);



			}

		}


	}






}
