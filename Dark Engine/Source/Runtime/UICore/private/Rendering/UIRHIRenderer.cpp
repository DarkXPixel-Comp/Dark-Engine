#include "Rendering/UIRHIRenderer.h"
#include "Math/MathFwd.h"
#include <DynamicRHI.h>


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