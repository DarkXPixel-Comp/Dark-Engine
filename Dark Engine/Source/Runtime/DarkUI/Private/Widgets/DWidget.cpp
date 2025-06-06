#include "Widgets/DWidget.h"

int32 DUIWidget::Paint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FRect4f& MyCullingRect, int32 LayerId)
{
	return 0;

}

void DUIWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

}

void DUIWidget::Invalidate(EInvalidateWidgetReason InvalidateReason)
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

void DUIWidget::OnToolTipClosing()
{

}

void DUIWidget::SetVisibility(TAttribute<EVisibility> InVisibility)
{
	Visibility = InVisibility;
}

void DUIWidget::ArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren, bool bInUpdateAttributes) const
{
	if (bInUpdateAttributes)
	{

	}

	OnArrangeChildren(AllottedGeometry, ArrangedChildren);
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
