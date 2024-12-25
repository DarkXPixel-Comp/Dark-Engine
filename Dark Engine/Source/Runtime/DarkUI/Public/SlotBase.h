#pragma once
#include "Platform/Platform.h"
#include "Memory/MemoryCore.h"



class DUIWidget;
class FChildren;

class FSlotBase
{
public:
	DarkUI_API FSlotBase();
	DarkUI_API FSlotBase(const FChildren& InParent);
	DarkUI_API FSlotBase(TSharedPtr<DUIWidget>& InWidget);

	FSlotBase(const FSlotBase&) = delete;

	FSlotBase& operator=(const FSlotBase) = delete;
	FSlotBase(const FSlotBase&) = delete;

	DarkUI_API virtual ~FSlotBase();

public:
	struct FSlotArguments {};

public:
	const FChildren* GetOwner() const
	{
		return Owner;
	}

	DarkUI_API DUIWidget* GetOwnerWidget() const;

	DarkUI_API void SetOwner(const FChildren& Children);

	void AttachWidget(TSharedPtr<DUIWidget>&& InWidget)


private:
	const FChildren* Owner;
	TSharedPtr<DUIWidget>& Widget;
};
