#pragma once
#include "Widgets/UIWindow.h"


class FUIRenderer
{
public:
	virtual void Initialize() {}
	virtual void CreateViewport(UIWindow* InWindow) = 0;
	virtual void DrawWindows(const TArray<TSharedPtr<UIWindow>>& Windows) = 0;

	bool IsViewportFullscreen(const UIWindow& Window) const;




};