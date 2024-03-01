#pragma once
#include "Widgets/UIWindow.h"


class FUIRenderer
{
public:
	virtual void Initialize() {}
	virtual void CreateViewport(UIWindow* InWindow) = 0;

	bool IsViewportFullscreen(const UIWindow& Window) const;




};