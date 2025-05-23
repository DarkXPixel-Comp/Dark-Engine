#include "Layout/ArrangedWidget.h"
#include "Widgets/DNullWidget.h"

FArrangedWidget::FArrangedWidget(const TSharedPtr<DUIWidget>& InWidget, const FGeometry& InGeometry) :
	Widget(InWidget),
	Geometry(InGeometry)
{


}

const FArrangedWidget& FArrangedWidget::GetNullWidget()
{
	static FArrangedWidget NullWidget(DUINullWidget::NullWidget, FGeometry());

	return NullWidget;
}

DARKUI_API DUIWidget* FArrangedWidget::GetWidgetPtr() const
{
	return Widget.get();
}

FWidgetAndPointer::FWidgetAndPointer() : FArrangedWidget(FArrangedWidget::GetNullWidget())
{

}

FWidgetAndPointer::FWidgetAndPointer(const FArrangedWidget& InWidget) :
	FArrangedWidget(InWidget)
{

}


