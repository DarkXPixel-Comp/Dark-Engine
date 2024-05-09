#pragma once
#include "Widgets/UIWidget.h"
#include "Delegate/Delegate.h"
#include "imgui.h"



DECLARE_MULTICAST_DELEGATE(FUIButtonDelegate);


class UIButton : public UIWidget
{
public:
	UIButton() {}
	virtual ~UIButton() = default;
	virtual void DrawImGui();
	virtual void Update(float DeltaTime);



	FUIButtonDelegate ButtonDelegate;
private:

};