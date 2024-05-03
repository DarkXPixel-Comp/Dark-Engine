#pragma once
#include "Widgets/UIWindow.h"


class FUIRenderer
{
public:
	virtual void Initialize() {}
	virtual void CreateViewport(UIWindow* InWindow) = 0;
	virtual void DrawWindows(const TArray<TSharedPtr<UIWindow>>& Windows) = 0;
	virtual void Resize(UIWindow* InWindow, const int32 Width, const int32 Height, const bool bWasMinimized) = 0;

	bool IsViewportFullscreen(const UIWindow& Window) const;

	void SetVsync(int32 ValueSync)
	{
		Vsync = ValueSync;
	}

protected:
	int32 Vsync = 1;



};