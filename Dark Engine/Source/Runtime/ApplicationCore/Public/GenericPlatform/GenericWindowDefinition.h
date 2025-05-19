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

struct FGenericWindowDefinition
{
	FString Title;
	
	float PosX = 0.f;
	float PosY = 0.f;

	float Width = 0.f;
	float Height = 0.f;

	bool bHasWindowBorder = true;

	int32 LeftBorder = 4;
	int32 TopBorder = 4;
	int32 RightBorder = 4;
	int32 BottomBorder = 4;
	

	bool bHasWindowPlatformBorder = true;

	bool bMovable = false;

	bool bSizable = false;

	int32 CornerRadius = 5;

	EWindowType Type;
};