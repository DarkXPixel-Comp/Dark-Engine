#pragma once
#include "Containers/DarkString.h"
#include "SlotBase.h"
#include "Widgets/InvalidateWidgetReason.h"
#include "Layout/BasicLayotWidgetSlot.h"

#include <optional>

class DUIWidget;

class FChildren
{
public:
	FChildren(DUIWidget* InOwner, const FString& InName = TEXT("Children")) :
		Owner(InOwner),
		Name(InName)
	{

	}

	FChildren(std::nullptr_t, const FString& InName = TEXT("Children")) = delete;

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

	virtual TSharedPtr<DUIWidget> GetChildAt(int32 Index) = 0;
	virtual TSharedPtr<const DUIWidget> GetChildAt(int32 Index) const = 0;

protected:
	struct FWidgetRef
	{
	private:
		std::optional<TSharedPtr<DUIWidget>> WidgetCopy;
		DUIWidget& WidgetReference;
	public:
		enum ECopyConstruct { CopyConstruct };
		enum ERefConstruct { ReferenceConstruct };

		FWidgetRef(ECopyConstruct, TSharedPtr<DUIWidget> InWidgetCopy) :
			WidgetCopy(std::move(InWidgetCopy)),
			WidgetReference(*WidgetCopy.value().get())
		{}

		FWidgetRef(ERefConstruct, DUIWidget& InWidgetRef) :
			WidgetCopy(),
			WidgetReference(InWidgetRef)
		{}
		FWidgetRef(const FWidgetRef& Other) : 
			WidgetCopy(Other.WidgetCopy),
			WidgetReference(WidgetCopy.has_value() ? *WidgetCopy.value().get() : Other.WidgetReference)
		{}

		FWidgetRef(FWidgetRef&& Other) :
			WidgetCopy(std::move(Other.WidgetCopy)),
			WidgetReference(WidgetCopy.has_value() ? *WidgetCopy.value().get() : Other.WidgetReference)
		{}

		FWidgetRef& operator=(const FWidgetRef&) = delete;
		FWidgetRef& operator=(FWidgetRef&&) = delete;

		DUIWidget& GetWidget() const
		{
			return WidgetReference;
		}




	};

private:
	FString Name;
	DUIWidget* Owner;

};

template<typename T>
class TSingleWidgetChildrenWithSlot : public FChildren, public TSlotBase<T>
{
public:
	TSingleWidgetChildrenWithSlot(DUIWidget* InOwner) :
		FChildren(InOwner),
		TSlotBase<T>(static_cast<const FChildren&>(*this))
	{

	}
	TSingleWidgetChildrenWithSlot(DUIWidget* InOwner, const FString& InName) :
		FChildren(InOwner, InName),
		TSlotBase<T>(static_cast<const FChildren&>(*this))
	{

	}

	TSingleWidgetChildrenWithSlot(std::nullptr_t) = delete;

public:
	virtual int32 Num() const override
	{
		return 1;
	}
	virtual TSharedPtr<DUIWidget> GetChildAt(int32 ChildIndex) override
	{
		return this->GetWidget();
	}

	virtual TSharedPtr<const DUIWidget> GetChildAt(int32 ChildIndex) const override
	{
		return this->GetWidget();
	}

	
public:
	struct FSlotArguments : public TSlotBase<T>::FSlotArguments
	{
		FSlotArguments() : 
			TSlotBase<T>::FSlotArguments(TSlotBase<T>::ConstructSlotIsFChildren)
		{}

		typename T::FSlotArguments& operator[](const TSharedPtr<DUIWidget>& InChildWidget)
		{
			TSlotBase<T>::FSlotArguments::AttachWidget(InChildWidget);
			return static_cast<typename T::FSlotArguments&>(*this);
		}
	};

public:
	using TSlotBase<T>::AttachWidget;
	using TSlotBase<T>::DetachWidget;
	using TSlotBase<T>::GetWidget;
	//using TSlotBase<T>::Invalidate;


	void Construct(FSlotArguments&& InArgs)
	{
		TSlotBase<T>::Construct(*this, std::move(InArgs));
	}

	T& operator[](const TSharedPtr<DUIWidget>& InChildWidget)
	{
		this->AttachWidget(AttachWidget);
		return static_cast<T&>(*this);
	}

	T& Expose(T*& OutVarToInit)
	{
		OutVarToInit = static_cast<T*>(this);
		return static_cast<T&>(*this);
	}

private:
	/*virtual const FSlotBase& GetSlotAt(int32 ChildIndex) const override
	{
		return *this;
	}

	virtual FWidgetRef GetChildRefAt(int32 ChildIndex) override
	{
		return FWidgetRef(FWidgetRef::ReferenceConstruct, *this->GetWidget().Get());
	}*/

};

class FSingleWidgetChildrenWithSlot : public TSingleWidgetChildrenWithSlot<FSingleWidgetChildrenWithSlot>
{
public:
	using TSingleWidgetChildrenWithSlot<FSingleWidgetChildrenWithSlot>::TSingleWidgetChildrenWithSlot;
};

template<EInvalidateWidgetReason InPaddingInvalidateReason = EInvalidateWidgetReason::Layout>
class TSingleWidgetChildrenWithBasicLayoutSlot :
	public TSingleWidgetChildrenWithSlot<TSingleWidgetChildrenWithBasicLayoutSlot<InPaddingInvalidateReason>>,
	public TPaddingSingleWidgetSlotMixin<TSingleWidgetChildrenWithBasicLayoutSlot<InPaddingInvalidateReason>, InPaddingInvalidateReason>
{
public:



};