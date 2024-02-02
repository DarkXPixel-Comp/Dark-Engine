#pragma once
#include "Engine/Engine.h"
#include "Widgets/UIWindow.h"


class DEditorEngine : public DEngine
{
public:
	static TSharedPtr<UIWindow>	CreateGameWindow();



};