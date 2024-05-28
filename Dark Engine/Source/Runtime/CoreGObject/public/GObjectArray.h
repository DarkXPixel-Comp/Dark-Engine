#pragma once
#include "HAL/Platform.h"
#include "Containers/Array.h"

class GObjectBase;


struct FGObjectItem
{
	GObjectBase* Object;
	int32 SerialNumber;
};


class FGObjectArray
{
	friend class GObject;

public:

	void AllocateObjectIndex(GObjectBase* Object);

	FGObjectItem* IndexToObject(int32 Index);

private:
	TArray<int32> AvailableList;
	TArray<FGObjectItem> Objects;


};


extern FGObjectArray GGObjectArray;