#pragma once

#include "Widgets/DCompoundWidget.h"


class DUIWindow : public DUICompoundWidget
{
public:
	DarkUI_BEGIN_ARGS( DUIWindow ) :
		_Title()
	{}
	DarkUI_ATTRIBUTE(FString, Title)

	DarkUI_END_ARGS()





};
