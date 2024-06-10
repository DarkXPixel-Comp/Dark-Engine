#pragma once
#include "HAL/Platform.h"


struct FColor
{
public:
	union 
	{
		struct
		{	
			uint8 R;
			uint8 G;
			uint8 B;
			uint8 A;
		};
		uint8 RGBA[4];
		uint32 Bits;
	};
};