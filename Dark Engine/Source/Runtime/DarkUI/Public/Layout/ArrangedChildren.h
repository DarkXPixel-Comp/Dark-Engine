#pragma once
#include "Layout/ArrangedWidget.h"
#include "Layout/Visibility.h"
#include "Platform/Platform.h"
#include "Containers/Array.h"


class FArrangedChildren
{
private:
	EVisibility VisibilityFilter;

public:
	typedef TArray<FArrangedWidget> FArrangedWidgetArray;

	FArrangedChildren(EVisibility InVisibilityFilter)
		: VisibilityFilter(InVisibilityFilter)
	{
	}
	
};