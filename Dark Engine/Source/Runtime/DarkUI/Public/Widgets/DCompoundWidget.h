#pragma once
#include "Widgets/DWidget.h"
#include "Layout/Children.h"



class DUICompoundWidget : public DUIWidget
{
	DUI_DECLARE_WIDGET_API(DUICompoundWidget, DUIWidget, DarkUI_API);

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
	DarkUI_API DUICompoundWidget() {}

	struct FCompoundWidgetOneChild : public TSingleWidgetChildrenWithSlot<int>
	{

	};


private:
	FVector2d ContentScale;


};
