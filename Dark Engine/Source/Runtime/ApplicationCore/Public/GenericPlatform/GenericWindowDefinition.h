#pragma once
#include "Containers/DarkString.h"
#include "Platform/Platform.h"


enum class EWindowType
{
	Normal,
	Menu,
	Tool,
	Notification,
	CursorDecorator,
	GameWindow
};

enum class EWindowMode
{
	FullScreen,
	WindowdFullscreen,
	Windowed
};


enum class EWindowZone
{
	NotInWindow = 0,
	TopLeftBorder = 1,
	TopBorder = 2,
	TopRightBorder = 3,
	LeftBorder = 4,
	ClientArea = 5,
	RightBorder = 6,
	BottomLeftBorder = 7,
	BottomBorder = 8,
	BottomRightBorder = 9,
	TitleBar = 10,
	MinimizeButton = 11,
	MaximizeButton = 12,
	CloseButton = 13,
	SysMenu = 14,

	Unspecified = 0
};

enum class EWindowActivationPolicy
{
	Never,
	Always,
	FirstShown
};

struct FWindowSizeLimits
{
public:
	FWindowSizeLimits& SetMinWidth(int32 InMinWidth)
	{
		MinWidth = InMinWidth;
		return *this;
	}

	FWindowSizeLimits& SetMinHeight(int32 InMinHeight)
	{
		MinHeight = InMinHeight;
		return *this;
	}

	FWindowSizeLimits& SetMaxWidth(int32 InMaxWidth)
	{
		MaxWidth = InMaxWidth;
		return *this;
	}

	FWindowSizeLimits& SetMaxHeight(int32 InMaxHeight)
	{
		MaxHeight = InMaxHeight;
		return *this;
	}

	int32 GetMinWidth() const
	{
		return MinWidth;
	}

	int32 GetMinHeight() const
	{
		return MinHeight;
	}

	int32 GetMaxWidth() const
	{
		return MaxWidth;
	}

	int32 GetMaxHeight() const
	{
		return MaxHeight;
	}

private:
	int32 MinWidth;
	int32 MinHeight;
	int32 MaxWidth;
	int32 MaxHeight;
};

struct FGenericWindowDefinition
{
	FString Title;
	EWindowType Type;
	EWindowMode Mode;

	float XDesiredPositionOnScreen;
	float YDisiredPositionOnScreen;

	float WidthDesiredOnScreen;
	float HeightDesiredOnScreen;

	bool bHasOSWindowBorder;
	bool bAcceptsInput;
	bool bIsTopmostWindow;
	bool bAppearsInTaskbar;
	bool bFocusWhenFirstShown;
	bool bHasCloseButton;
	bool bSupportMinimize;
	bool bSupportMaximize;

	bool bIsModalWindow;
	bool bIsRegularWindow;
	bool bHasSizingFrame;
	bool bSizeWillChangeOften;
	bool bShouldPreserveAspectRatio;

	int32 ExpectedMaxWidth;
	int32 ExpectedMaxHeight;

	float Opacity;
	int32 CornerRadius;
	FWindowSizeLimits SizeLimits;
	EWindowActivationPolicy ActivationPolicy;
	bool bManualDPI;
};