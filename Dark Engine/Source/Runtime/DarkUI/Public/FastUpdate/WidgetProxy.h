#pragma once
#include "Platform/Platform.h"


class FDarkUIInvalidationRoot
{

};

struct FDarkUIInvalidationRootHandle
{
public:
	FDarkUIInvalidationRootHandle();
	FDarkUIInvalidationRootHandle(int32 UniqueID);

	int32 GetUniqueID() const;

private:
};

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

	DarkUI_API bool IsValid(const DUIWidget& Widget) const;
	DarkUI_API bool IsValid(const DUIWidget* Widget) const;


};