#include "GObjectArray.h"
#include "ObjectBase.h"

FGObjectArray GGObjectArray;




void FGObjectArray::AllocateObjectIndex(GObjectBase* Object)
{
	int32 Index = -1;

	if (AvailableList.Num() > 0)
	{
		Index = AvailableList.PopBack();
	}
	else
	{
		Objects.Add(FGObjectItem());
		Index = Objects.Num() - 1;
	}


	FGObjectItem* ObjectItem = IndexToObject(Index);

	ObjectItem->Object = Object;
	ObjectItem->SerialNumber = 0;
	Object->InternalIndex = Index;




}

FGObjectItem* FGObjectArray::IndexToObject(int32 Index)
{
	if (Index < Objects.Num())
	{
		return &Objects[Index];
	}

	return nullptr;
}
