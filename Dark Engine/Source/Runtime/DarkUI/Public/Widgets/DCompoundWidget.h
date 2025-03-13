#pragma once
#include "Widgets/DWidget.h"
#include "Layout/Children.h"



class DUICompoundWidget : public DUIWidget
{
	DUI_DECLARE_WIDGET_API(DUICompoundWidget, DUIWidget, DARKUI_API);

public:
	const FVector2d GetContentScale() const
	{
		return ContentScale;
	}

	void SetContentScale(const FVector2d InContentScale)
	{
		ContentScale = InContentScale;
	}


protected:
	DARKUI_API DUICompoundWidget() {}

private:
	FVector2d ContentScale;


};
