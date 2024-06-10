#pragma once
#include "imgui.h"
#include "Widgets/UIWidget.h"
#include "Widgets/UIButton.h"
#include "Delegate/Delegate.h"

DECLARE_MULTICAST_DELEGATE(FUIMenuDelegate);

DECLARE_MULTICAST_DELEGATE_OneParam(FUIMenuItemDelegate, bool);

class UIMenu : public UIWidget
{
public:
	UIMenu() {}
	virtual ~UIMenu() = default;

	virtual void DrawImGui();
	virtual void Update(float DeltaTime) override;




	FUIMenuDelegate	MenuDelegate;
private:




};



class UIMenuItem : public UIWidget
{
public:
	UIMenuItem() {}

	virtual void DrawImGui();
	virtual void Update(float DeltaTime);

	void SetDependWidget(TSharedPtr<UIWidget> InWidget)
	{
		DependWidget = InWidget;
	}



	FUIMenuItemDelegate MenuItemDelegate;
private:
	bool bPressed = false;
	bool bLastPressed = false;
	TSharedPtr<UIWidget> DependWidget;

};