#pragma once
#include "Types/Attributes/DAttributeBase.h"
#include "Misc/Defines.h"

class FDarkUIAttributeDescriptor
{
public:
	struct FInvalidateWidgetReasonAttribute
	{
		friend class FDarkUIAttributeDescriptor;
		using FGetter = TDelegate<EInvalidateWidgetReason, const class DUIWidget&>;

		FInvalidateWidgetReasonAttribute(const FInvalidateWidgetReasonAttribute&) = default;
		FInvalidateWidgetReasonAttribute(FInvalidateWidgetReasonAttribute&&) = default;
		FInvalidateWidgetReasonAttribute& operator=(const FInvalidateWidgetReasonAttribute&) = default;
		FInvalidateWidgetReasonAttribute& operator=(FInvalidateWidgetReasonAttribute&&) = default;

		explicit FInvalidateWidgetReasonAttribute(EInvalidateWidgetReason InReason)	:
			Reason(InReason),
			Getter()
		{}

		template<typename... PayloadTypes>
		explicit FInvalidateWidgetReasonAttribute(std::function<EInvalidateWidgetReason(const class DUIWidget&)> InFunction) :
			Reason(EInvalidateWidgetReason::None)
		{
			Getter.Bind(InFunction);
		}

		bool IsBound() const
		{
			return Getter.IsBound();
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

	struct FAttribute;
	struct FContainer;
	struct FContainerInitializer;
	struct FInitializer;

	struct FContainer
	{
		friend FInitializer;

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
	public:
		friend FDarkUIAttributeDescriptor;
		friend FInitializer;
		friend FContainerInitializer;
		

		FAttribute(const FString& InName, uint32 InOffset, FInvalidateWidgetReasonAttribute InReason);
		FAttribute(const FString InContainerName, const FString& InName, uint32 InOffset, FInvalidateWidgetReasonAttribute InReason);

		FString GetName() const
		{
			return Name;
		}

		uint32 GetSortOrder() const
		{
			return SortOrder;
		}

		EInvalidateWidgetReason GetInvalidationReason(const DUIWidget& Widget) const
		{
			return InvalidationReason.Get(Widget);
		}

		EDarkUIAttributeType GetAttributeType() const
		{
			return AttributeType;
		}

		bool IsAffectVisibility() const
		{
			return bAffectVisibility;
		}

		void ExecuteOnValueChangedIfBound(DUIWidget& Widget) const
		{
			return OnValueChanged.BroadCast(Widget);
		}

	private:
		FString Name;
		uint32 Offset;
		FString Prerequisite;
		FString ContainerName;
		uint32 SortOrder;
		uint8 ContainerIndex = 0;
		FInvalidateWidgetReasonAttribute InvalidationReason;
		FAttributeValueChangedDelegate OnValueChanged;
		EDarkUIAttributeType AttributeType;
		bool bAffectVisibility;
	};

	struct FContainerInitializer
	{
	private:
		friend FDarkUIAttributeDescriptor;
		DarkUI_API FContainerInitializer(FDarkUIAttributeDescriptor& InDescriptor, const FString& ContainerName);
		DarkUI_API FContainerInitializer(FDarkUIAttributeDescriptor& InDescriptor, const FDarkUIAttributeDescriptor& ParentDescriptor, const FString& ContainerName);
	
	public:
		FContainerInitializer() = delete;
		DELETE_COPY(FContainerInitializer);

		struct FAttributeEntry
		{
			DarkUI_API FAttributeEntry(FDarkUIAttributeDescriptor& Descriptor, const FString& ContainerName, int32 AttributeIndex);

			DarkUI_API FAttributeEntry& UpdatePrerequisite(const FString& Prerequisite);

			DarkUI_API FAttributeEntry& OnValueChanged(FAttributeValueChangedDelegate Callback);

		private:
			FDarkUIAttributeDescriptor& Descriptor;
			FString ContainerName;
			int32 AttributeIndex;
		};

		DarkUI_API FAttributeEntry AddContainedAttribute(const FString& AttributeName, uint32 Offset, const FInvalidateWidgetReasonAttribute& Reason);

		DarkUI_API FAttributeEntry AddContainedAttribute(const FString& AttributeName, uint32 Offset, FInvalidateWidgetReasonAttribute&& Reason);

		DarkUI_API void OverrideInvalidationReason(const FString& AttributeName, const FInvalidateWidgetReasonAttribute& Reason);

		DarkUI_API void OverrideInvalidationReason(const FString& AttributeName, FInvalidateWidgetReasonAttribute&& Reason);

		DarkUI_API void OverrideOnValueChanged(const FString& AttributeName, ECallbackOverrideType OverrideType, FAttributeValueChangedDelegate Callback);
	
	private:
		FDarkUIAttributeDescriptor& Descriptor;
		FString ContainerName;

	};

	struct FInitializer
	{
		DarkUI_API FInitializer(FDarkUIAttributeDescriptor& InDescriptor);
		DarkUI_API FInitializer(FDarkUIAttributeDescriptor& InDescriptor, const FDarkUIAttributeDescriptor& ParentDescriptor);
		
		DELETE_COPY(FInitializer);

	public:
		DarkUI_API ~FInitializer();

		struct FAttributeEntry
		{
			DarkUI_API FAttributeEntry(FDarkUIAttributeDescriptor& Descriptor, int32 InAttributeIndex);

			DarkUI_API FAttributeEntry& UpdatePrerequisite(const FString& Prerequisite);

			DarkUI_API FAttributeEntry& AffectVisibility();

			DarkUI_API FAttributeEntry& OnValueChanged(FAttributeValueChangedDelegate Callback);

		private:
			FDarkUIAttributeDescriptor& Descriptor;
			int32 AttributeIndex;
		};

		DarkUI_API FAttributeEntry AddMemberAttribute(const FString& AttributeName, uint32 Offset, const FInvalidateWidgetReasonAttribute& Reason);
		DarkUI_API FAttributeEntry AddMemberAttribute(const FString& AttributeName, uint32 Offset, FInvalidateWidgetReasonAttribute&& Reson);

		DarkUI_API FContainerInitializer AddContainer(const FString ContainerName, uint32 Offset);
		
	public:
		DarkUI_API void OverrideInvalidationReason(const FString& AttributeName, const FInvalidateWidgetReasonAttribute& Reason);

		DarkUI_API void OverrideInvalidationReason(const FString& AttributeName, FInvalidateWidgetReasonAttribute&& Reason);

		DarkUI_API void OverrideOnValueChanged(const FString& AttributeName, ECallbackOverrideType OverrideType, FAttributeValueChangedDelegate Callback);

		DarkUI_API void SetAffectVisibility(const FString& AttributeName, bool bAffectVisibility);
	private:
		FDarkUIAttributeDescriptor& Descriptor;
	};

	uint64 GetAttributeNum() const
	{
		return Attributes.Num();
	}

	DarkUI_API const FAttribute& GetAttributeAtIndex(int32 Index) const;

	DarkUI_API const FContainer* FindContainer(const FString& ContainerName) const;

	DarkUI_API const FAttribute* FindAttribute(const FString& AttributeName) const;

	DarkUI_API const FAttribute* FindMemberAttribute(uint32 AttributeOffset) const;

	DarkUI_API const FAttribute* FindContainedAttribute(const FString& ContainerName, uint32 AttributeOffset) const;

	DarkUI_API int32 IndexOfContainer(const FString& ContainerName) const;

	DarkUI_API int32 IndexOfAttribute(const FString& AttributeName) const;

	DarkUI_API int32 IndexOfMemberAttribute(uint32 AttributeOffset) const;
																		 
	DarkUI_API int32 IndexOfContainedAttribute(const FString& ContainerName, uint32 AttributeOffset) const;

private:
	DarkUI_API FAttribute* FindAttribute(const FString& AttributeName);

	DarkUI_API FContainerInitializer AddContainer(const FString& ContainerName, uint32 Offset);

	DarkUI_API FInitializer::FAttributeEntry AddMemberAttribute(const FString& AttributeName, uint32 Offset, FInvalidateWidgetReasonAttribute ReasonGetter);

	DarkUI_API FContainerInitializer::FAttributeEntry AddContainedAttribute(const FString& ContainerName, const FString& AttributeName, uint32 Offset, FInvalidateWidgetReasonAttribute ReasonGetter);

	DarkUI_API void OverrideInvalidationReason(const FString& ContainerName, const FString& AttributeName, FInvalidateWidgetReasonAttribute ReasonGetter);

	DarkUI_API void OverrideOnValueChanged(const FString& ContainerName, const FString& AttributeName, ECallbackOverrideType OverrideType, FAttributeValueChangedDelegate Callback);

	DarkUI_API void SetPrerequisite(const FString& ContainerName, FAttribute& Attribute, const FString& Prerequisite);

	DarkUI_API void SetAffectVisibility(FAttribute& Attribute, bool bUpdate);

private:
	TArray<FDarkUIAttributeDescriptor::FAttribute> Attributes;
	TArray<FContainer> Containers;

};