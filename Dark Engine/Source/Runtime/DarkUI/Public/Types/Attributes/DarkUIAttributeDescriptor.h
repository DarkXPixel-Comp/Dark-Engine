#pragma once
#include "Types/Attributes/DAttributeBase.h"

class FDarkUIAttributeDescriptor
{
public:
	struct FInvalidateWidgetReasonAttribute
	{
		friend class FDarkUIAttributeDescriptor;
		using FGetter = TDelegate<EInvalidateWidgetReason, const class DUIWidget&>;

		explicit FInvalidateWidgetReasonAttribute(EInvalidateWidgetReason InReason)	:
			Reason(InReason),
			Getter()
		{}

		template<typename... PayloadTypes>
		explicit FInvalidateWidgetReasonAttribute(std::function<EInvalidateWidgetReason(const class DUIWidget&)>&& InFunction) :
			Reason(EInvalidateWidgetReason::None)
		{
			Getter.Bind(InFunction);
		}

		bool IsBound() const
		{
			return Getter.container.get();
		}

		EInvalidateWidgetReason Get(const class DUIWidget& Widget) const
		{
			return IsBound() ? Getter.BroadCast(Widget) : Reason;;
		}

	private:
		EInvalidateWidgetReason Reason;
		FGetter Getter;
	};

	using FAttributeValueChangedDelegate = TDelegate<void, DUIWidget&>;

	enum class ECallbackOverrideType
	{
		ReplacePrevious,
		ExecuteAfterPrevious,
		ExecuteBeforePrevious
	};

public:
	static constexpr uint32 DefaultSortOrder(uint32 Offsert) { return Offsert * 100; }

	struct FContainer
	{
		FContainer() = default;
		FContainer(FString InName, uint32 InOffset)	: 
			Name(InName),
			Offset(InOffset)
		{}

		bool IsValid() const
		{
			return Name.Lenght();
		}

		FString GetName() const
		{
			return Name;
		}

		uint32 GetSortOrder() const
		{
			return SortOrder;
		}

	private:
		FString Name;
		uint32 Offset = 0;
		uint32 SortOrder = 0;
	};

	struct FAttribute
	{
		FAttribute(const FString& Name, uint32 Offset, FInvalidateWidgetReasonAttribute Reason);
		FAttribute(const FString ContainerName, const FString& Name, uint32 Offset, FInvalidateWidgetReasonAttribute Reason);
	};

};