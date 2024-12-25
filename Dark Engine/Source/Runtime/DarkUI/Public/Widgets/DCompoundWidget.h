#pragma once
#include "Widgets/DWidget.h"



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
	DarkUI_API DUICompoundWidget();

	//struct FCompoundWidgetOneChild



private:
	FVector2d ContentScale;


};
