#pragma once
#include "Types/DarkUIMetaData.h"
#include "Types/Attributes/DAttributeBase.h"
#include "Misc/Defines.h"


class DUIWidget;

class FDarkUIAttributeMetaData : public IDarkUIMetaData
{
public:
	DUI_METADATA_TYPE(FDarkUIAttributeMetaData, IDarkUIMetaData);

	FDarkUIAttributeMetaData() = default;
	DELETE_COPY(FDarkUIAttributeMetaData);

public:
	static DarkUI_API FDarkUIAttributeMetaData* FindMetaData(const DUIWidget& OwningWidget);
	
	enum class EInvalidationPermission : uint8
	{
		AllowInvalidationIfConstructed,
		AllowInvalidation,
		DelayInvalidation,
		DenyInvalidation,
		DenyAndClearDelayedInvalidation
	};

	static DarkUI_API void UpdateAllAttributes(DUIWidget& OwningWidget, EInvalidationPermission InvalidationStyle);

	static DarkUI_API void UpdateOnlyVisibilityAttributes(DUIWidget& OwningWidget, EInvalidationPermission InvalidationStyle);

	static DarkUI_API void UpdateExceptVisibilityAttributes(DUIWidget& OwningWidget, EInvalidationPermission InvalidationStyle);

	static DarkUI_API void UpdateChildrenOnlyVisibilityAttributes(DUIWidget& OwningWidget, EInvalidationPermission InvalidationStyle, bool bIsRecursive);

	static DarkUI_API void ApplyDelayedInvalidation(DUIWidget& OwningWidget);


public:
	bool IsBound(const FDarkUIAttributeBase& Attribute) const
	{
		return IndexOfAttribute(Attribute) != -1;
	}


private:
	struct FGetterItem
	{
		DELETE_COPY(FGetterItem);
		FGetterItem(FDarkUIAttributeBase* InAttribute, uint32 InSortOrder, TUniquePtr<IDarkUIAttributeGetter>&& InGetter, const )
	};

private:

	int32 IndexOfAttribute(const FDarkUIAttributeBase& Attribute) const
	{
		const FDarkUIAttributeBase* AttributePtr = &Attribute;
	}
};