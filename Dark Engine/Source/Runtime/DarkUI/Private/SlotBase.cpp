#include "SlotBase.h"
#include "Widgets/DWidget.h"

void FSlotBase::Invalidate(EInvalidateWidgetReason InvalidateReason)
{
	if (DUIWidget* OwnerWidget = GetOwnerWidget())
	{
		OwnerWidget->Invalidate(InvalidateReason);
	}
}
