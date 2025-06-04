#pragma once
#include "Widgets/DCompoundWidget.h"
#include "Layout/Children.h"
#include "Layout/Margin.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"


enum class ESizingRule : uint8
{
	FixedSize,

	AutoSize,

	UserSize
};


class DUIWindow : public DUICompoundWidget
{
public:
	DarkUI_BEGIN_ARGS( DUIWindow ) :
		_Title(),
		_Type(EWindowType::Normal),
		_UserResizeBorder(5),
		_SizingRule(ESizingRule::UserSize),
		_bHasOSBorder(false),
		_bIsRegularWindow(true),
		_IsInitiallyMaximized(false),
		_IsInitiallyMinimized(false),
		_ShouldPreserveAspectRatio(false)
	{}

	DarkUI_ATTRIBUTE(FString, Title)
	DarkUI_ARGUMENT(FVector2f, ScreenPosition)
	DarkUI_ARGUMENT(FVector2f, Size)
	DarkUI_ARGUMENT(EWindowType, Type)
	DarkUI_ARGUMENT(bool, AdjustInitialSizeAndPositionForDPIScale)
	DarkUI_ARGUMENT(bool, IsInitiallyMaximized)
	DarkUI_ARGUMENT(bool, IsInitiallyMinimized)
	DarkUI_ARGUMENT(FMargin, UserResizeBorder)
	DarkUI_ARGUMENT(ESizingRule, SizingRule)
	DarkUI_ARGUMENT(bool, bHasOSBorder)
	DarkUI_ARGUMENT(bool, bHasCloseButton)
	DarkUI_ARGUMENT(bool, bSupportMaximize)
	DarkUI_ARGUMENT(bool, bSupportMinimize)
	DarkUI_ARGUMENT(bool, IsPopupWindow)
	DarkUI_ARGUMENT(bool, IsTopmostWindow)
	DarkUI_ARGUMENT(bool, FocusWhenFirstShown)
	DarkUI_ARGUMENT(bool, bIsRegularWindow)
	DarkUI_ARGUMENT(bool, ShouldPreserveAspectRatio)
	DarkUI_END_ARGS()


public:
	DARKUI_API void Construct(const FArguments& InArgs);

	DARKUI_API void ResizeWindowSize(FVector2f NewWindowSize);

	DARKUI_API EWindowType GetType() const
	{
		return Type;
	}


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

	float GetDPIScale() const;

	DARKUI_API bool IsVisible() const;

	DARKUI_API bool AcceptsInput() const;

	DARKUI_API bool IsScreenSpaceMouseWithin(FVector2d MousePosition) const;

	DARKUI_API FVector2d GetSize() const;

	DARKUI_API FVector2d GetPosition() const;

	DARKUI_API FVector2d GetInitialDesiredSize() const;
	DARKUI_API FVector2d GetInitialDesiredPosition() const;

	DARKUI_API bool HasOSBorder() const;
	DARKUI_API bool IsRegularWindow() const;
	DARKUI_API bool ShouldPreserveAspectRation() const;

	DARKUI_API void SetCachedSize(FVector2f NewSize);
	DARKUI_API void SetCachedPosition(const FVector2f NewPosition);



public:
	std::shared_ptr<const DUIWindow> shared_from_this()	const
	{
		return std::static_pointer_cast<const DUIWindow>(DUICompoundWidget::shared_from_this());
	}

	std::shared_ptr<DUIWindow> shared_from_this()
	{
		return std::static_pointer_cast<DUIWindow>(DUICompoundWidget::shared_from_this());
	}


protected:

	DARKUI_API void InitialMinimize();
	DARKUI_API void InitialMaximize();


protected:
	TAttribute<FString> Title;
	EWindowType Type;

	FMargin UserResizeBorder;
	ESizingRule SizingRule;

	bool bHasOSBorder : 1;
	bool bHasEverBeenShown : 1;
	bool bIsRegularWindow : 1;
	bool bInitialMaximize : 1;
	bool bInitialMinimize : 1;
	bool bShouldPreserveAspectRation : 1;

	TSharedPtr<FGenericWindow> NativeWindow;
	FVector2d Size;
	FVector2d ScreenPosition;
	FVector2d InitialDesiredScreenPosition;
	FVector2d InitialDesiredSize;

	mutable EWindowZone WindowZone;

	TArray<TSharedPtr<DUIWindow>> ChildWindows;

	TWeakPtr<DUIWindow> ParentWindow;



};
