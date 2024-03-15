#pragma once
#include "Widgets/UIWidget.h"


class UIDock : public UIWidget
{
public:
	UIDock() {}

	void DrawImGui();
	void Update(float DeltaTime);




};