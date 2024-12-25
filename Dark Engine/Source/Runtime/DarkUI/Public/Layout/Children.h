#pragma once
#include "Containers/DarkString.h"


class DUIWidget;

class FChildren
{
public:
	FChildren(DUIWidget* InOwner, const FString& InName = TEXT("Children")) :
		Owner(InOwner),
		Name(InName)
	{

	}

	FChildren(std::nullptr_t) = delete;

	virtual ~FChildren() = default;

	void* operator new(size_t) = delete;

	void* operator new[](size_t) = delete;

	virtual int32 Num() const = 0;

	DUIWidget& GetOwner() const
	{
		return *Owner;
	}

	virtual int32 NumSlot() const
	{
		return Num();
	}

private:
	FString Name;
	DUIWidget* Owner;

};

template<typename SlotType>
class TSingleWidgetChildrenWithSlot : public FChildren
{

};