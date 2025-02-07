#include "Widgets/DWidget.h"

void DUIWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

}

DarkUI_API void DUIWidget::Invalidate(EInvalidateWidgetReason InvalidateReason)
{
	if (InvalidateReason == EInvalidateWidgetReason::None || !IsConstructed())
	{
		return;
	}


	if (EnumHasAnyFlags(InvalidateReason, EInvalidateWidgetReason::Prepass))
	{
		MarkPrepassAsDirty();
		InvalidateReason |= EInvalidateWidgetReason::Layout;
	}

	if (EnumHasAnyFlags(InvalidateReason, EInvalidateWidgetReason::ChildOrder) || false)
	{
		MarkPrepassAsDirty();
		InvalidateReason |= EInvalidateWidgetReason::Prepass;
		InvalidateReason |= EInvalidateWidgetReason::Layout;
	}

	const bool bVolatilityChanged = EnumHasAnyFlags(InvalidateReason, EInvalidateWidgetReason::Volatility) ? _InvalidateVolatility() : false;




}

void DUIWidget::DUIWidgetConstruct(const FDarkUIBaseNamedArgs& Args)
{
	




}

bool DUIWidget::_InvalidateVolatility()
{
	const bool bWasDirectlyVolatile = IsVolatile();
	CacheVoltatility();
	return bWasDirectlyVolatile != IsVolatile();
}
