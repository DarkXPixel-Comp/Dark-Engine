#pragma once
#include "Math/Color.h"


enum class EDUIColorStylingMode
{
	Specified,
	ColorTable,
	Foreground,
	Foreground_Subdued,
	UseStyle
};

struct FDUIColor
{
public:
	


protected:
	FLinearColor SpecifiedColor;
	EDUIColorStylingMode ColorRule;
};