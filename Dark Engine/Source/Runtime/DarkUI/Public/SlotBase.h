#pragma once
#include "Platform/Platform.h"
#include "Memory/MemoryCore.h"
#include <Widgets/InvalidateWidgetReason.h>
#include "Misc/Attribute.h"




class DUIWidget;
class FChildren;

class FSlotBase
{
public:
	DARKUI_API FSlotBase();
	DARKUI_API FSlotBase(const FChildren& InParent);
	DARKUI_API FSlotBase(TSharedPtr<DUIWidget>& InWidget);

	FSlotBase(const FSlotBase&) = delete;

	FSlotBase& operator=(const FSlotBase) = delete;

	DARKUI_API virtual ~FSlotBase();

public:
	struct FSlotArguments {};

public:
	const FChildren* GetOwner() const
	{
		return Owner;
	}

	DARKUI_API DUIWidget* GetOwnerWidget() const { return nullptr; }

	DARKUI_API void SetOwner(const FChildren& Children);

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

	DARKUI_API TSharedPtr<DUIWidget> DetachWidget()
	{
		return Widget;
	}

	DARKUI_API void Invalidate(EInvalidateWidgetReason InvalidateReason);

protected:
	template<typename TargetValueType, typename SourceValueType>
	bool SetAttribute(TAttribute<TargetValueType>& TargetValue, const TAttribute<SourceValueType>& SourceValue, EInvalidateWidgetReason InvalidateReason)
	{
		if (!TargetValue.IdenticalTo(SourceValue))
		{
			const bool bWasBound = TargetValue.IsBound();
			const bool bBoudlessChanged = bWasBound != SourceValue.IsBound();

			TargetValue = SourceValue;
			if (bBoudlessChanged)
			{
				InvalidateReason |= EInvalidateWidgetReason::Volatility;
			}
			Invalidate(InvalidateReason);
			return true;

		}
		return false;
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