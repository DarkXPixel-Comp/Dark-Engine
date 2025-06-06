#pragma once
#include "Math/MathFwd.h"


struct FLinearColor
{
	union
	{
		struct
		{
			float R;
			float G;
			float B;
			float A;
		};
		float RGBA[4];
	};


};