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

FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry::FAttributeEntry(FDarkUIAttributeDescriptor& InDescriptor, int32 InAttributeIndex):
	Descriptor(InDescriptor),
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

 void FDarkUIAttributeDescriptor::FContainerInitializer::OverrideOnValueChanged(const FString& AttributeName, ECallbackOverrideType OverrideType, FAttributeValueChangedDelegate Callback)
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
	struct FPrerequisiteSort
	{						
		FPrerequisiteSort() = default;
		FPrerequisiteSort(int32 InAttributeIndex, int32 InPrerequisiteIndex, int32 InDepth)	:
			AttributeIndex(InAttributeIndex),
			PrerequisiteIndex(InPrerequisiteIndex),
			Depth(InDepth)
		{
		}




		void CalculateDepth(TArray<FPrerequisiteSort> Prerequisites)
		{
			if (Depth < 0)
			{
				if (Prerequisites[PrerequisiteIndex].Depth < 0)
				{
					Prerequisites[PrerequisiteIndex].CalculateDepth(Prerequisites);
				}
				Depth = Prerequisites[PrerequisiteIndex].Depth + 1;
			}
		}

		int32 AttributeIndex;
		int32 PrerequisiteIndex;
		int32 Depth = -1;

	};




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

	TArray<FPrerequisiteSort> Prerequisites;
	Prerequisites.Reserve(Descriptor.Attributes.Num());

	bool bSortNeeded = false;

	for (int32 Index = 0; Index < Descriptor.Attributes.Num(); ++Index)
	{
		FAttribute& Attribute = Descriptor.Attributes[Index];

		if (Attribute.ContainerName != TEXT(""))
		{
			const int32 FoundIndex = (const int32)Descriptor.Containers.IndexOfByPredicate([ContainerName = Attribute.ContainerName](const FContainer& Other) -> bool
				{
					return Other.GetName() == ContainerName;
				});
			if (FoundIndex == -1)
				return;
			Attribute.ContainerIndex = (uint8)FoundIndex;
			Attribute.SortOrder = Descriptor.Containers[FoundIndex].SortOrder + (Attribute.Offset * 10);
		}
		else
		{
			Attribute.ContainerIndex = UINT8_MAX;
			Attribute.SortOrder = DefaultSortOrder(Attribute.Offset);
		}

		if (Attribute.Prerequisite != TEXT(""))
		{
			const FString Prerequisite = Attribute.Prerequisite;
			const int32 PrerequisiteIndex = (const int32)Descriptor.Attributes.IndexOfByPredicate([&Prerequisite](const FAttribute& Other) -> bool
				{
					return Other.GetName() == Prerequisite;
				});
			if (Descriptor.Attributes.IsValidIndex(PrerequisiteIndex))
			{
				Prerequisites.Emplace(Index, PrerequisiteIndex, -1);
				bSortNeeded = true;
			}
			else
			{
				Prerequisites.Emplace(Index, -1, 0);
			}
		}
		else
		{
			int32 PrerequisiteIndex = -1;
			if (Index != 0 && Attribute.bAffectVisibility)
			{
				bSortNeeded = true;
				PrerequisiteIndex = 0;
			}
			Prerequisites.Emplace(Index, PrerequisiteIndex, 0);
		}
	}

	if (bSortNeeded)
	{
		for (FPrerequisiteSort& PrerequisiteSort : Prerequisites)
		{
			PrerequisiteSort.CalculateDepth(Prerequisites);
		}

		const TArray<FAttribute>& Attributes = Descriptor.Attributes;

		Prerequisites.Sort([&Attributes](const FPrerequisiteSort& A, const FPrerequisiteSort& B) -> bool
			{
				const FAttribute& AttributeA = Attributes[A.AttributeIndex];
				const FAttribute& AttributeB = Attributes[B.AttributeIndex];

				if (AttributeA.ContainerIndex != AttributeB.ContainerIndex)
				{
					if (AttributeA.ContainerName == TEXT(""))
					{
						return true;
					}
					else if (AttributeB.ContainerName == TEXT(""))
					{
						return false;
					}
					return AttributeA.ContainerIndex < AttributeB.ContainerIndex;
				}
				if (AttributeA.bAffectVisibility != AttributeB.bAffectVisibility)
				{
					return AttributeA.bAffectVisibility;
				}
				if (A.Depth != B.Depth)
				{
					return A.Depth < B.Depth;
				}

				if (A.PrerequisiteIndex == B.PrerequisiteIndex)
				{
					return Attributes[A.AttributeIndex].SortOrder < Attributes[B.AttributeIndex].SortOrder;
				}
				const int32 SortA = A.PrerequisiteIndex != -1 ? Attributes[A.PrerequisiteIndex].SortOrder : AttributeA.SortOrder;
				const int32 SortB = B.PrerequisiteIndex != -1 ? Attributes[B.PrerequisiteIndex].SortOrder : AttributeB.SortOrder;
				return SortA < SortB;
			});
		int32 PreviousPrerequisiteIdnex = -1;
		int32 IncreaseCount = 1;
		for (const FPrerequisiteSort& Element : Prerequisites)
		{
			if (Element.PrerequisiteIndex != -1)
			{
				if (PreviousPrerequisiteIdnex == Element.PrerequisiteIndex)
				{
					++IncreaseCount;
				}
				Descriptor.Attributes[Element.AttributeIndex].SortOrder = Descriptor.Attributes[Element.PrerequisiteIndex].SortOrder + IncreaseCount;
			}
			PreviousPrerequisiteIdnex = Element.PrerequisiteIndex;
		}

		Descriptor.Attributes.Sort([](const FAttribute& A, const FAttribute& B) -> bool
			{
				return A.SortOrder < B.SortOrder;
			});
	}
}

FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry& FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry::UpdatePrerequisite(const FString& Prerequisite)
{
	if (Descriptor.Attributes.IsValidIndex(AttributeIndex))
	{
		Descriptor.SetPrerequisite(TEXT(""), Descriptor.Attributes[AttributeIndex], Prerequisite);
	}
	return *this;
}

FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry& FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry::AffectVisibility()
{
	if (Descriptor.Attributes.IsValidIndex(AttributeIndex))
	{
		Descriptor.SetAffectVisibility(Descriptor.Attributes[AttributeIndex], true);
	}
	return *this;
}


FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry& FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry::OnValueChanged(FAttributeValueChangedDelegate Callback)
{
	if (Descriptor.Attributes.IsValidIndex(AttributeIndex))
	{
		Descriptor.Attributes[AttributeIndex].OnValueChanged = std::move(Callback);
	}
	return *this;
}

FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry FDarkUIAttributeDescriptor::FInitializer::AddMemberAttribute(const FString& AttributeName, uint32 Offset, const FInvalidateWidgetReasonAttribute& Reason)
{
	return Descriptor.AddMemberAttribute(AttributeName, Offset, Reason);
}

FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry FDarkUIAttributeDescriptor::FInitializer::AddMemberAttribute(const FString& AttributeName, uint32 Offset, FInvalidateWidgetReasonAttribute&& Reason)
{
	return Descriptor.AddMemberAttribute(AttributeName, Offset, std::move(Reason));
}

FDarkUIAttributeDescriptor::FContainerInitializer FDarkUIAttributeDescriptor::FInitializer::AddContainer(const FString ContainerName, uint32 Offset)
{
	return Descriptor.AddContainer(ContainerName, Offset);
}

 void FDarkUIAttributeDescriptor::FInitializer::OverrideInvalidationReason(const FString& AttributeName, const FInvalidateWidgetReasonAttribute& Reason)
{
	Descriptor.OverrideInvalidationReason(TEXT(""), AttributeName, Reason);
}

 void FDarkUIAttributeDescriptor::FInitializer::OverrideInvalidationReason(const FString& AttributeName, FInvalidateWidgetReasonAttribute&& Reason)
{
	Descriptor.OverrideInvalidationReason(TEXT(""), AttributeName, std::move(Reason));
}

 void FDarkUIAttributeDescriptor::FInitializer::OverrideOnValueChanged(const FString& AttributeName, ECallbackOverrideType OverrideType, FAttributeValueChangedDelegate Callback)
{
	Descriptor.OverrideOnValueChanged(TEXT(""), AttributeName, OverrideType, Callback);
}

 void FDarkUIAttributeDescriptor::FInitializer::SetAffectVisibility(const FString& AttributeName, bool bAffectVisibility)
{
	FAttribute* Attribute = Descriptor.FindAttribute(AttributeName);
	if (Attribute)
	{
		Descriptor.SetAffectVisibility(*Attribute, bAffectVisibility);
	}
}

const FDarkUIAttributeDescriptor::FAttribute& FDarkUIAttributeDescriptor::GetAttributeAtIndex(int32 Index) const
{
	FDarkUIAttributeDescriptor::FAttribute const& Result = Attributes[Index];
	return Result;
}

const FDarkUIAttributeDescriptor::FContainer* FDarkUIAttributeDescriptor::FindContainer(const FString& ContainerName) const
{
	return Containers.FindByPredicate([&ContainerName](const FContainer& Other) -> bool
		{
			return Other.GetName() == ContainerName;
		});
}

const FDarkUIAttributeDescriptor::FAttribute* FDarkUIAttributeDescriptor::FindAttribute(const FString& AttributeName) const
{
	return Attributes.FindByPredicate([&AttributeName](const FAttribute& Other) { return Other.Name == AttributeName; });
}

const FDarkUIAttributeDescriptor::FAttribute* FDarkUIAttributeDescriptor::FindMemberAttribute(uint32 AttributeOffset) const
{
	return Attributes.FindByPredicate([AttributeOffset](const FAttribute& Other)
		{
			return Other.Offset == AttributeOffset &&
				Other.AttributeType == EDarkUIAttributeType::Member;
		});
}

const FDarkUIAttributeDescriptor::FAttribute* FDarkUIAttributeDescriptor::FindContainedAttribute(const FString& ContainerName, uint32 AttributeOffset) const
{
	return Attributes.FindByPredicate([&ContainerName, AttributeOffset](const FAttribute& Other) 
		{
			return Other.Offset == AttributeOffset &&
				Other.AttributeType == EDarkUIAttributeType::Contained &&
				Other.ContainerName == ContainerName;
		});
}

int32 FDarkUIAttributeDescriptor::IndexOfContainer(const FString& ContainerName) const
{
	return (int32)Containers.IndexOfByPredicate([&ContainerName](const FContainer& Other) { return Other.GetName() == ContainerName; });
}

int32 FDarkUIAttributeDescriptor::IndexOfAttribute(const FString& AttributeName) const
{
	return (int32)Attributes.IndexOfByPredicate([&AttributeName](const FAttribute& Other) { return Other.Name == AttributeName; });
}

int32 FDarkUIAttributeDescriptor::IndexOfMemberAttribute(uint32 AttributeOffset) const
{
	return (int32)Attributes.IndexOfByPredicate([AttributeOffset](const FAttribute& Other)
		{
			return Other.Offset == AttributeOffset &&
				Other.AttributeType == EDarkUIAttributeType::Member; 
		});;
}

int32 FDarkUIAttributeDescriptor::IndexOfContainedAttribute(const FString& ContainerName, uint32 AttributeOffset) const
{
	return (int32)Attributes.IndexOfByPredicate([&ContainerName, AttributeOffset](const FAttribute& Other)
		{
			return Other.Offset == AttributeOffset &&
				Other.AttributeType == EDarkUIAttributeType::Contained &&
				Other.ContainerName == ContainerName;
		});;
}

FDarkUIAttributeDescriptor::FAttribute* FDarkUIAttributeDescriptor::FindAttribute(const FString& AttributeName)
{
	return Attributes.FindByPredicate([&AttributeName](const FAttribute& Other) { return Other.Name == AttributeName; });
}

FDarkUIAttributeDescriptor::FContainerInitializer FDarkUIAttributeDescriptor::AddContainer(const FString& ContainerName, uint32 Offset)
{
	const FContainer* Container = FindContainer(ContainerName);
	if (!Container)
	{
		Containers.Emplace(ContainerName, Offset);
	}
	return FContainerInitializer(*this, ContainerName);
}


FDarkUIAttributeDescriptor::FInitializer::FAttributeEntry FDarkUIAttributeDescriptor::AddMemberAttribute(const FString& AttributeName, uint32 Offset, FInvalidateWidgetReasonAttribute ReasonGetter)
{
	int32 NewIndex = -1;
	FAttribute const *Attribute = FindAttribute(AttributeName);
	if (!Attribute)
	{
		NewIndex = (int32)Attributes.Emplace_GetIndex(AttributeName, Offset, std::move(ReasonGetter));
	}
	return FInitializer::FAttributeEntry(*this, NewIndex);
}

FDarkUIAttributeDescriptor::FContainerInitializer::FAttributeEntry FDarkUIAttributeDescriptor::AddContainedAttribute(const FString& ContainerName, const FString& AttributeName, uint32 Offset, FInvalidateWidgetReasonAttribute ReasonGetter)
{
	int32 NewIndex = -1;
	FAttribute const *Attribute = FindAttribute(AttributeName);
	if (!Attribute)
	{
		NewIndex = (int32)Attributes.Emplace_GetIndex(ContainerName, AttributeName, Offset, std::move(ReasonGetter));
	}
	return FContainerInitializer::FAttributeEntry(*this, ContainerName, NewIndex);
}

void FDarkUIAttributeDescriptor::OverrideInvalidationReason(const FString& ContainerName, const FString& AttributeName, FInvalidateWidgetReasonAttribute ReasonGetter)
{
	FAttribute* Attribute = FindAttribute(AttributeName);
	if (Attribute && Attribute->ContainerName == ContainerName)
	{
		Attribute->InvalidationReason = std::move(ReasonGetter);
	}

}

void FDarkUIAttributeDescriptor::OverrideOnValueChanged(const FString& ContainerName, const FString& AttributeName, ECallbackOverrideType OverrideType, FAttributeValueChangedDelegate Callback)
{
	FAttribute* Attribute = FindAttribute(AttributeName);
	if (Attribute && Attribute->ContainerName == ContainerName)
	{
		switch (OverrideType)
		{
		case FDarkUIAttributeDescriptor::ECallbackOverrideType::ReplacePrevious:
			Attribute->OnValueChanged = std::move(Callback);
			break;
		case FDarkUIAttributeDescriptor::ECallbackOverrideType::ExecuteAfterPrevious:
		case FDarkUIAttributeDescriptor::ECallbackOverrideType::ExecuteBeforePrevious:
			if (Attribute->OnValueChanged.IsBound() && Callback.IsBound())
			{
				FAttributeValueChangedDelegate Previous = Attribute->OnValueChanged;
				Attribute->OnValueChanged.Bind([Previous{ std::move(Previous) }, Callback{ std::move(Callback) }, OverrideType](DUIWidget& Widget)
					{
						if (OverrideType == ECallbackOverrideType::ExecuteBeforePrevious)
						{
							Previous.BroadCast(Widget);
						}
						Callback.BroadCast(Widget);
						if (OverrideType == ECallbackOverrideType::ExecuteAfterPrevious)
						{
							Previous.BroadCast(Widget);
						}
					});
			}
			else if (Callback.IsBound())
			{
				Attribute->OnValueChanged = std::move(Callback);
			}
			break;
		}
	}
}

void FDarkUIAttributeDescriptor::SetPrerequisite(const FString& ContainerName, FAttribute& Attribute, const FString& InPrerequisite)
{
	if (InPrerequisite == TEXT(""))
	{
		Attribute.Prerequisite = TEXT("");
	}
	else
	{
		const FAttribute* Prerequisite = FindAttribute(InPrerequisite);
		if (Prerequisite && Prerequisite->ContainerName == ContainerName)
		{
			Attribute.Prerequisite = InPrerequisite;
			
			TArray<FString> Recursion;
			Recursion.Reserve(Attributes.Num());
			FAttribute const *CurrentAttribute = &Attribute;
			while (CurrentAttribute->Prerequisite != TEXT(""))
			{
				if (Recursion.Contains(CurrentAttribute->Name))
				{
					Attribute.Prerequisite = TEXT("");
					break;
				}
				Recursion.Add(CurrentAttribute->Name);
				CurrentAttribute = FindAttribute(CurrentAttribute->Prerequisite);
			}
		}
		else
		{
			Attribute.Prerequisite = TEXT("");
		}
	}
}

void FDarkUIAttributeDescriptor::SetAffectVisibility(FAttribute& Attribute, bool bUpdate)
{
	Attribute.bAffectVisibility = bUpdate;
}	 
