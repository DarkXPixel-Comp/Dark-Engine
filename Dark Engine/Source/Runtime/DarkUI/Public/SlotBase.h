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
	{
		Widget = std::move(InWidget);
	}

	void AttachWidget(const TSharedPtr<DUIWidget>& InWidget)
	{
		Widget = InWidget;
	}

	const TSharedPtr<DUIWidget>& GetWidget() const
	{
		return Widget;
	}

	DarkUI_API TSharedPtr<DUIWidget> DetachWidget()
	{
		return Widget;
	}


private:
	const FChildren* Owner;
	TSharedPtr<DUIWidget> Widget;
};

template<typename T>
class TSlotBase : public FSlotBase
{
public:
	T& operator[](TSharedPtr<DUIWidget>&& InChildWidget)
	{
		this->AttachWidget(std::move(InChildWidget));
		return static_cast<T&>(*this);
	}

	T& operator[](const TSharedPtr<DUIWidget>& InChildWidget)
	{
		this->AttachWidget(InChildWidget);
		return static_cast<T&>(*this);
	}

	enum EConstructSlotIsFChildren { ConstructSlotIsFChildren };

	struct FSlotArguments : public FSlotBase::FSlotArguments
	{
	public:
		FSlotArguments(EConstructSlotIsFChildren) {}
		FSlotArguments(TUniquePtr<T> InSlot) :
			Slot(std::move(InSlot))
		{}
	public:
		typename T::FSlotArguments& operator[](TSharedPtr<DUIWidget>&& InChildWidget)
		{
			ChildWidget = std::move(InChildWidget);
			return Me();
		}

		typename T::FSlotArguments& operator[](TSharedPtr<DUIWidget>& InChildWidget)
		{
			ChildWidget = InChildWidget;
			return Me();
		}

		void AttachWidget(const TSharedPtr<DUIWidget>& InChildWidget)
		{
			ChildWidget = InChildWidget;
		}

		const TSharedPtr<DUIWidget>& GetAttachedWidget() const { return ChildWidget; }

		T* GetSlot() const { return Slot.get(); }

		typename T::FSlotArguments& Me()
		{
			return static_cast<typename T::FSlotArguments&>(*this);
		}

	private:
		TUniquePtr<T> Slot;
		TSharedPtr<DUIWidget> ChildWidget;
	};

	void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs)
	{
		if (InArgs.GetAttachedWidget())
		{
			AttachWidget(InArgs.GetAttachedWidget());
		}
		SetOwner(SlotOwner);
	}
};