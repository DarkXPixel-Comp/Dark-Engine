#pragma once
#include "Platform/Platform.h"


struct FDarkUIInvalidationWidgetIndex
{
	friend class FDarkUIInvalidationWidgetList;
	friend struct FDarkUIInvalidationWidgetSortOrder;

public:
	static const FDarkUIInvalidationWidgetIndex Invalid;


private:
	FDarkUIInvalidationWidgetIndex(uint16 InArray, uint16 InElement) : ArrayIndex(InArray), ElementIndex(InElement)
	{ }

	uint16 ArrayIndex;
	uint16 ElementIndex;

};


class FWidgetProxyHandle
{
	friend class DUIWidget;
public:
	FWidgetProxyHandle() {}
};