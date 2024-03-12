#pragma once
#include "UIWidget.h"
#include "imgui.h"


class UIMainMenuBar : public UIWidget
{
public:
	UIMainMenuBar() {}

	virtual void DrawImGui();

	virtual void Update(float DeltaTime);

	int32 GetRecommendedHeightObject() const { return Size.Y; }


private:
	bool IsRects() const;





};