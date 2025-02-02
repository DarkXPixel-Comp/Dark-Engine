#include "Types/Attributes/DarkUIAttributeDescriptor.h"

FDarkUIAttributeDescriptor::FAttribute::FAttribute(const FString& InName, uint32 InOffset, FInvalidateWidgetReasonAttribute InReason):
	Name(InName),
	Offset(InOffset),
	SortOrder(DefaultSortOrder(InOffset)),
	InvalidationReason(std::move(InReason)),
	AttributeType(EDarkUIAttributeType::Member),
	bAffectVisibility(false)
{
}

FDarkUIAttributeDescriptor::FAttribute::FAttribute(const FString InContainerName, const FString& InName, uint32 InOffset, FInvalidateWidgetReasonAttribute InReason) :
	Name(InName),
	Offset(InOffset),
	ContainerName(InContainerName),
	SortOrder(DefaultSortOrder(InOffset)),
	InvalidationReason(std::move(InReason)),
	AttributeType(EDarkUIAttributeType::Contained),
	bAffectVisibility(false)
{
}

FDarkUIAttributeDescriptor::FContainerInitializer::FContainerInitializer(FDarkUIAttributeDescriptor& InDescriptor, const FString& InContainerName)  :
	Descriptor(InDescriptor),
	ContainerName(InContainerName)
{

}

FDarkUIAttributeDescriptor::FContainerInitializer::FContainerInitializer(FDarkUIAttributeDescriptor& InDescriptor, const FDarkUIAttributeDescriptor& ParentDescriptor, const FString& InContainerName) :
	Descriptor(InDescriptor),
	ContainerName(InContainerName)
{
	for (const FDarkUIAttributeDescriptor::FAttribute& Attribute : ParentDescriptor.Attributes)
	{
		if (Attribute.ContainerName == InContainerName)
		{
			bool bAlready = InDescriptor.Attributes.Contains([AttName = Attribute.GetName()](const FDarkUIAttributeDescriptor::FAttribute& Other)-> bool
				{
					return Other.GetName() == AttName;
				});

			if (!bAlready)
			{
				FAttribute att(Attribute);
			}
		}
	}

}

FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry::FAttributeEntry(FDarkUIAttributeDescriptor& InDescriptor, const FString& InContainerName, int32 InAttributeIndex)	:
	Descriptor(InDescriptor),
	ContainerName(InContainerName),
	AttributeIndex(InAttributeIndex)
{

}

FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry& FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry::UpdatePrerequisite(const FString& Prerequisite)
{
	if (Descriptor.Attributes.IsValidIndex(AttributeIndex))
	{
		Descriptor.SetPrerequisite(ContainerName, Descriptor.Attributes[AttributeIndex], Prerequisite);
	}
	return *this;
}

//FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry& FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry::AffectVisibility()
//{
//	if (Descriptor.Attributes.IsValidIndex(AttributeIndex))
//	{
//		Descriptor.SetAffectVisibility(Descriptor.Attributes[AttributeIndex], true);
//	}
//	return *this;
//}

FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry& FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry::OnValueChanged(FAttributeValueChangedDelegate InCallback)
{
	if (Descriptor.Attributes.IsValidIndex(AttributeIndex))
	{
		Descriptor.Attributes[AttributeIndex].OnValueChanged = std::move(InCallback);
	}
	return *this;
}

FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry FDarkUIAttributeDescriptor::FContainerInitializer::AddContainedAttribute(const FString& AttributeName, uint32 Offset, const FInvalidateWidgetReasonAttribute& Reason)
{
	return Descriptor.AddContainedAttribute(ContainerName, AttributeName, Offset, Reason);
}

FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry FDarkUIAttributeDescriptor::FContainerInitializer::AddContainedAttribute(const FString& AttributeName, uint32 Offset, FInvalidateWidgetReasonAttribute&& Reason)
{
	return Descriptor.AddContainedAttribute(ContainerName, AttributeName, Offset, Reason);
}

void FDarkUIAttributeDescriptor::FContainerInitializer::OverrideInvalidationReason(const FString& AttributeName, const FInvalidateWidgetReasonAttribute& Reason)
{
	Descriptor.OverrideInvalidationReason(ContainerName, AttributeName, Reason);
}

void FDarkUIAttributeDescriptor::FContainerInitializer::OverrideInvalidationReason(const FString& AttributeName, FInvalidateWidgetReasonAttribute&& Reason)
{
	Descriptor.OverrideInvalidationReason(ContainerName, AttributeName, Reason);
}

DarkUI_API void FDarkUIAttributeDescriptor::FContainerInitializer::OverrideOnValueChanged(const FString& AttributeName, ECallbackOverrideType OverrideType, FAttributeValueChangedDelegate Callback)
{
	Descriptor.OverrideOnValueChanged(ContainerName, AttributeName, OverrideType, Callback);
}

FDarkUIAttributeDescriptor::FInitializer::FInitializer(FDarkUIAttributeDescriptor& InDescriptor):
	Descriptor(InDescriptor)
{
}

FDarkUIAttributeDescriptor::FInitializer::FInitializer(FDarkUIAttributeDescriptor& InDescriptor, const FDarkUIAttributeDescriptor& ParentDescriptor) :
	Descriptor(InDescriptor)
{
	InDescriptor.Attributes = ParentDescriptor.Attributes;
	InDescriptor.Containers = ParentDescriptor.Containers;
}

FDarkUIAttributeDescriptor::FInitializer::~FInitializer()
{
	if (Descriptor.Containers.Num() > 0)
	{
		for (FContainer& Container : Descriptor.Containers)
		{
			Container.SortOrder = DefaultSortOrder(Container.Offset);
		}
		Descriptor.Containers.Sort([](const FContainer& Value1, const FContainer& Value2) -> bool
			{
				return Value1.GetSortOrder() < Value2.GetSortOrder();
			});
	}
}
