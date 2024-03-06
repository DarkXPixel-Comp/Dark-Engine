#include "Rendering/UIRHIRenderer.h"
#include "Math/MathFwd.h"
#include <DynamicRHI.h>
#include "RHICommandList.h"


void FUIRHIRenderer::Initialize()
{




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

void FUIRHIRenderer::DrawWindows(const TArray<TSharedPtr<UIWindow>>& InWindows)
{
	FRHICommandListImmediate* RHICmdList = nullptr;

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


				RHICmdList->BeginDrawingViewport(ViewInfo->ViewportRHI.get(), nullptr);
				//RHICmdList->SetViewport(0, 0, 0 ViewInfo->Width, ViewInfo->Height, 0.f);




			}

		}


	}






}
