#pragma once
#include "Widgets/DCompoundWidget.h"
#include "Layout/Children.h"


class DUIWindow : public DUICompoundWidget
{
public:
	DarkUI_BEGIN_ARGS( DUIWindow ) :
		_Title()
	{}
	DarkUI_ATTRIBUTE(FString, Title)

	DarkUI_END_ARGS()


public:
	DARKUI_API void Construct(const FArguments& InArgs);





};
