#pragma once
#include "Platform/Platform.h"
#include "Misc/EnumClassFlags.h"


enum class EInvalidateWidgetReason : uint8
{
	None = 0,

	Layout = 1 << 0,

	Paint = 1 << 1,

	Volatility = 1 << 2,

	ChildOrder = 1 << 3,

	RenderTransform = 1 << 4,

	Visibility = 1 << 5,

	AttributeRegistration = 1 << 6,

	Prepass = 1 << 7,

	PaintAndVolatility = Paint | Volatility,

	LayoutAndVolatility = Layout | Volatility
};

ENUM_CLASS_FLAGS(EInvalidateWidgetReason);

