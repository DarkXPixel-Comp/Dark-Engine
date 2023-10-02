#pragma once
#include <HAL/Platform.h>




class FPrimitiveComponentId
{
public:
	FPrimitiveComponentId() : PrimValue(0)
	{}

	bool isValid() const
	{
		return PrimValue > 0;
	}

	bool operator==(FPrimitiveComponentId otherId)
	{
		return PrimValue == otherId.PrimValue;
	}


	uint32 PrimValue;
};