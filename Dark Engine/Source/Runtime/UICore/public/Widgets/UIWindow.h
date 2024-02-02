#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "Memory/TUniquePtr.h"


class UIWindow
{
public:
	UIWindow();

public:
	void ShowWindow();



private:
	TSharedPtr<FGenericWindow> NativeWindow;




};