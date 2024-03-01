#include "Rendering/UIRenderer.h"

bool FUIRenderer::IsViewportFullscreen(const UIWindow& Window) const
{
	bool bFullScreen = false;

	if (GIsEditor)
	{
		bFullScreen = false;
	}
	else
	{
		bFullScreen = Window.GetNativeWindow()->GetWindowMode() == EWindowMode::Fullscreen;
	}

	return bFullScreen;
}