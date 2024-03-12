#pragma once
#include "imgui.h"
#include "Widgets/UIWidget.h"
#include "Widgets/UIButton.h"
#include "Delegate/Delegate.h"

DECLARE_DELEGATE(FUIMenuDelegate);



class UIMenu : public UIWidget
{
public:
	UIMenu() {}

	virtual void DrawImGui();
	virtual void Update(float DeltaTime) override;




	FUIMenuDelegate	MenuDelegate;
private:




};