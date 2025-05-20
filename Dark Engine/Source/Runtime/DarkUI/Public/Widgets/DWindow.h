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
	DarkUI_ARGUMENT(FVector2f, Size)
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

	DARKUI_API void SetScreenPosition(const FVector2f& NewPosition);
	DARKUI_API void SetSize(FVector2f NewSize);

	DARKUI_API void SetNativeWindow(const TSharedPtr<FGenericWindow>& InNativeWindow);

	DARKUI_API FVector2f GetInitialDesiredSizeInScreen() const;

	DARKUI_API FVector2f GetInitialDesiredPositionInScreen() const;

	DARKUI_API void ShowWindow();

	DARKUI_API TArray<TSharedPtr<DUIWindow>>& GetChildWindows();

	DARKUI_API TSharedPtr<FGenericWindow> GetNativeWindow() const;

	DARKUI_API bool IsSubWindowOf(const TSharedPtr<DUIWindow>& ParentWindow) const;

	DARKUI_API TSharedPtr<DUIWindow> GetParentWindow() const;

	DARKUI_API EWindowZone GetWindowZoneForPoint(const FVector2f& Point) const;

	EWindowMode GetWindowMode() const { return NativeWindow->GetWindowMode(); }

protected:
	DARKUI_API void SetCachedSize(FVector2f NewSize);


protected:
	TAttribute<FString> Title;
	EWindowType Type;
	bool bHasOSBorder : 1;
	bool bHasEverBeenShown : 1;


	TSharedPtr<FGenericWindow> NativeWindow;
	FVector2f Size;
	FVector2f ScreenPosition;
	FVector2f InitialDesiredScreenPosition;
	FVector2f InitialDesiredSize;



	TArray<TSharedPtr<DUIWindow>> ChildWindows;

	TWeakPtr<DUIWindow> ParentWindow;



};
