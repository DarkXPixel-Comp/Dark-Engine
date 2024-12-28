#pragma once
#include "SlotBase.h"
#include "Widgets/InvalidateWidgetReason.h"
#include "Widgets/DWidget.h"
#include <type_traits>


template <typename MixedIntoType, EInvalidateWidgetReason InPaddingInvalidationReason = EInvalidateWidgetReason::Layout>
class TPaddingSingleWidgetSlotMixin
{
public:
	template<typename WidgetType, typename V = typename std::enable_if<std::is_base_of<DUIWidget, WidgetType>::value>::type>
	TPaddingSingleWidgetSlotMixin(WidgetType& InParent)
	{

	}
};