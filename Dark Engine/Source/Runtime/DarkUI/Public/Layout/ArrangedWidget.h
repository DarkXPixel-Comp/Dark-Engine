#pragma once
#include "Platform/Platform.h"
#include "Layout/Geometry.h"
#include "Memory/MemoryCore.h"

class DUIWidget;



class FArrangedWidget
{	
public:
	FArrangedWidget(const TSharedPtr<DUIWidget>& InWidget, const FGeometry& InGeometry);


	DARKUI_API static const FArrangedWidget& GetNullWidget();

	DARKUI_API DUIWidget* GetWidgetPtr() const;

	bool operator==(const FArrangedWidget& Other) const
	{
		return Widget == Other.Widget;
	}

private:
	FGeometry Geometry;
	TSharedPtr<DUIWidget> Widget;
};


struct DARKUI_API FWidgetAndPointer : public FArrangedWidget
{
public:
	FWidgetAndPointer();

	FWidgetAndPointer(const FArrangedWidget& InWidget);

	//FWidgetAndPointer(const FArrangedWidget& InWidget)
};