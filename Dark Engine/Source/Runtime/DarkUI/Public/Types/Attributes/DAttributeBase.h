#pragma once

#include <type_traits>
#include "Platform/Platform.h"
#include "Widgets/InvalidateWidgetReason.h"
#include "Widgets/DWidget.h"


enum class EDarkUIAttributeType : uint8
{
	Member = 0,
	Managet = 1,
	Contained = 2

};


struct FDarkUIAttributeBase
{
	template<typename T>
	constexpr static bool IsInvalidateWidgetReasonSupported(T Reason)
	{
		return false;
	}

	constexpr static bool IsInvalidateWidgetReasonSupported(EInvalidateWidgetReason Reason)
	{
		return (Reason & (EInvalidateWidgetReason::ChildOrder | EInvalidateWidgetReason::AttributeRegistration)) == EInvalidateWidgetReason::None;
	}
};


class IDarkUIAttributeGetter
{
public:
	struct FUpdateAttributeResult
	{
		FUpdateAttributeResult(EInvalidateWidgetReason InInvalidateReason) :
			InvalidateReason(InInvalidateReason),
			bInvalidateRequested(true)
		{ }

		FUpdateAttributeResult() :
			InvalidateReason(EInvalidateWidgetReason::None),
			bInvalidateRequested(false)
		{}

		EInvalidateWidgetReason InvalidateReason;
		bool bInvalidateRequested;
	};

	virtual FUpdateAttributeResult UpdateAttribute(const DUIWidget& Widget) = 0;
	virtual const FDarkUIAttributeBase& GetAttribute() const = 0;
	virtual void SetAttribute(FDarkUIAttributeBase&) = 0;
};

struct FDarkUIAttributeImpl : public FDarkUIAttributeBase
{
protected:
	DarkUI_API void ProtectedUnregisterAttribute(DUIWidget& Widget, EDarkUIAttributeType AttributeType) const;
	DarkUI_API void ProtectedRegisterAttribute(DUIWidget& Widget, EDarkUIAttributeType AttributeType, TUniquePtr<IDarkUIAttributeGetter>&& Wrapper);
	DarkUI_API void ProtectedInvalidateWidget(DUIWidget& Widget, EDarkUIAttributeType AttributeType) const;
	DarkUI_API bool ProtectedIsBound(const DUIWidget& Widget, EDarkUIAttributeType AttributeType) const;
	DarkUI_API IDarkUIAttributeGetter* ProtectedFindGetter(const EDarkUIAttributeType AttributeType) const;


};

template<typename AttributeMemberType>
struct TDarkUIAttributeRef
{
private:
	std::weak_ptr<const DUIWidget> Owner;
	const AttributeMemberType& Attribute;
};
													
struct FDarkUIAttributeNoInvalidationReason
{
	static constexpr EInvalidateWidgetReason GetInvalidationReason(const DUIWidget&) { return EInvalidateWidgetReason::None; }
};																					


class IDarkUIAttributeContainer
{
public:
	virtual DUIWidget& GetContainerWidget() const = 0;
	virtual FString GetContainerName() const = 0;
	virtual uint32 GetContainerSortOrder() const = 0;
protected:
	DarkUI_API void RemoveContainerWidget(DUIWidget& Widget);
	DarkUI_API void UpdateContainerSortOrder(DUIWidget& Widget);
};


template<typename ContainerType, typename InObjectType, typename InInvalidateReasonPredicate, typename InComparePredicateType, EDarkUIAttributeType InAttributeType>
class TDarkUIAttributeBase : public FDarkUIAttributeImpl
{
public:
	template<typename AttributeMemberType>
	friend struct TDarkUIAttributeRef;

	using ObjectType = InObjectType;

	static const EDarkUIAttributeType AttributeType = InAttributeType;
	static constexpr bool HasDefinedInvalidationReason = !std::is_same<InInvalidateReasonPredicate, FDarkUIAttributeNoInvalidationReason>::value;
	static_assert(std::is_same<ContainerType, DUIWidget>::value || std::is_same<ContainerType, IDarkUIAttributeContainer>::value);

public:
	const ObjectType& Get() const
	{
		return Value;
	}

	void UpdateNow(ContainerType& Widget)
	{

	}



private:
	ObjectType Value;


};
