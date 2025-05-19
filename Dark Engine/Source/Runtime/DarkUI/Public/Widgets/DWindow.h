#pragma once
#include "Widgets/DCompoundWidget.h"
#include "Layout/Children.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"



class DUIWindow : public DUICompoundWidget
{
public:
	DarkUI_BEGIN_ARGS( DUIWindow ) :
		_Title(),
		_Type(EWindowType::Normal)
	{}

	DarkUI_ATTRIBUTE(FString, Title)
	DarkUI_ARGUMENT(FVector2f, ScreenPosition)
	DarkUI_ARGUMENT(EWindowType, Type)
	DarkUI_ARGUMENT(bool, AdjustInitialSizeAndPositionForDPIScale)
	DarkUI_ARGUMENT(bool, IsInitiallyMaximized)
	DarkUI_ARGUMENT(bool, IsInitiallyMinimized)
	DarkUI_END_ARGS()


public:
	DARKUI_API void Construct(const FArguments& InArgs);

	DARKUI_API void ResizeWindowSize(FVector2f NewWindowSize);

	DARKUI_API EWindowType GetType() const
	{
		return Type;
	}

protected:
	DARKUI_API void SetCachedSize(FVector2f NewSize);


protected:
	TAttribute<FString> Title;
	EWindowType Type;
	TSharedPtr<FGenericWindow> NativeWindow;



};
