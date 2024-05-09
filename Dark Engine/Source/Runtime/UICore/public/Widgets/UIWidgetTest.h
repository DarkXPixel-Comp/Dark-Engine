#pragma once

#include "Widgets/UIWidget.h"


class UIWidgetTest : public UIWidget
{
public:
	UIWidgetTest() {}
	virtual ~UIWidgetTest() = default;
	void DrawImGui();
	void Update(float DeltaTime);



private:





};