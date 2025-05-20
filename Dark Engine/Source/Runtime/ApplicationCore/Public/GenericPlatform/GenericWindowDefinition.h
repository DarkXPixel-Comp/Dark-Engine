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

struct FGenericWindowDefinition
{
	FString Title;
	
	float PosX = 0.f;
	float PosY = 0.f;

	float Width = 0.f;
	float Height = 0.f;

	bool bHasOSWindowBorder = true;

	int32 LeftBorder = 4;
	int32 TopBorder = 4;
	int32 RightBorder = 4;
	int32 BottomBorder = 4;
	

	bool bHasWindowPlatformBorder = true;

	bool bMovable = false;

	bool bSizable = false;

	int32 CornerRadius = 5;

	EWindowType Type;

	EWindowMode Mode;

	bool AppearsInTaskbar;

	bool IsTopmostWindow;

	bool AcceptsInput;

	bool IsRegularWindow;

	bool FocusWhenFirstShown;
	bool HasCloseButton;
	bool SupportsMinimize;
	bool SupportsMaximize;

	bool IsModalWindow;
	bool HasSizingFrame;
	bool ShouldPreserveAspectRatio;

	bool bSizeWillChangeOften;
	int32 ExpectedMaxWidth;
	int32 ExpectedMaxHeight;
};