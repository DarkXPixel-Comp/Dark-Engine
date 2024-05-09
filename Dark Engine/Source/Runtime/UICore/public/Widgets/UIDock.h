#pragma once
#include "Widgets/UIWidget.h"


class UIDock : public UIWidget
{
public:
	UIDock(): UIWidget(TEXT("UIDock")) {}
	virtual ~UIDock() = default;

	void DrawImGui();
	void Update(float DeltaTime);

	void SetMainMenuBar(TSharedPtr<class UIMainMenuBar> InMenuBar);

private:
	TSharedPtr<class UIMainMenuBar>	MainMenuBar;



};