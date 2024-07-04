#include "Rendering/UIRHIRenderer.h"
#include "Math/MathFwd.h"
#include <DynamicRHI.h>
#include "RHICommandList.h"
#include <CoreGlobals.h>
#include "Console/GlobalInputOutConsole.h"
#include "ImGuizmo.h"
#include "DirectXMath.h"
#include <future>


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


FUIRHIRenderer::~FUIRHIRenderer()
{
	//WindowToViewportInfo.clear();
	for (auto& i : WindowToViewportInfo)
	{
		//i.second->ViewportRHI->AddRef();
		i.second->ViewportRHI.Reset();
	}
}

void FUIRHIRenderer::DestroyViewport(UIWindow* InWindow)
{
	auto It = WindowToViewportInfo.find(InWindow);

	if (It != WindowToViewportInfo.end())
	{
		/*FRHIViewport* Temp = It->second->ViewportRHI.get();
		Temp->AddRef();*/
		WindowToViewportInfo.erase(It);
	}

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
	auto It = WindowToViewportInfo.find(InWindow);

	if (It != WindowToViewportInfo.end())
	{
		FViewportInfo* Info = It->second;
		Info->bFullscreen = IsViewportFullscreen(*InWindow);
		Info->Width = Width;
		Info->Height = Height;
		Info->ViewportRHI->Resize(Width, Height, bWasMinimized);
	}
}

using namespace DirectX;

void FUIRHIRenderer::DrawWindows(const TArray<TSharedPtr<UIWindow>>& InWindows)
{
	OPTICK_EVENT();
	FRHICommandListImmediate* RHICmdList = &GRHICommandList.GetImmediateCommandList();

	for (auto& Window : InWindows)
	{
		if (Window)
		{
			const FVector2f ViewportSize = Window->GetViewportSize();

			if (ViewportSize.X > 0 && ViewportSize.Y > 0)
			{
				FViewportInfo* ViewInfo = nullptr;


				auto It = WindowToViewportInfo.find(Window.get());
				//FViewportInfo* FoundViewInfo = WindowToViewportInfo.find(Window.get())->second;
				if (It != WindowToViewportInfo.end())
				{
					ViewInfo = It->second;
				}
				else
				{
					continue;
				}

				bool bVsync = 1;


				FRHITexture* BackBuffer = RHIGetViewportBackBuffer(ViewInfo->ViewportRHI.Get());
				if (!BackBuffer)
				{
					return;
				}
				RHICmdList->BeginDrawingViewport(ViewInfo->ViewportRHI.Get(), nullptr);
				RHICmdList->BeginFrame();
#ifdef IMGUI
				RHICmdList->BeginImGui();
				Window->GetNativeWindow()->BeginImGui();

				FRHIRenderPassInfo RPInfo(BackBuffer, ERenderPassMode::Clear, ERenderPassMode::Preserve);
				RHICmdList->BeginRenderPass(RPInfo);
				ImGui::NewFrame();
				ImGuizmo::BeginFrame();
				ImGuizmo::Enable(true);
#endif
				for (auto& Widget : Window->GetWidgets())
				{
					Widget->DrawImGui();
				}
#ifdef IMGUI
				ImGui::Render();
				RHICmdList->EndImGui();
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}
#endif

				RHICmdList->EndRenderPass(RPInfo);
				//RHICmdList->EndFrame();
				RHICmdList->EndDrawingViewport(ViewInfo->ViewportRHI.Get(), true, Vsync);



			}

		}


	}






}
