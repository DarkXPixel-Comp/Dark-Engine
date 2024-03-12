#pragma once
#include "Widgets/UIWidget.h"
#include "Delegate/Delegate.h"
#include "imgui.h"



DECLARE_MULTICAST_DELEGATE(FUIButtonDelegate);


class UIButton : public UIWidget
{
public:
	UIButton() {}
	virtual void DrawImGui();


	FUIButtonDelegate ButtonDelegate;
private:
};