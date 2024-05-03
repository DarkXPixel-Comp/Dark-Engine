#pragma once

#include "Widgets/UIWidget.h"

enum class EUISide
{
	Left,
	Right,
	Up,
	Down
};



class UISideWidget : public UIWidget
{
public:
	UISideWidget(EUISide InSide);

	virtual void DrawImGui() override;


private:
	EUISide Side;



};