#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "Memory/TUniquePtr.h"
#include "CoreMinimal.h"


class UIWindow
{
public:
	UIWindow();

public:
	void ShowWindow();




public:
	FString Title;


	TSharedPtr<FGenericWindow> GetNativeWindow() { return NativeWindow; }
	void SetNativeWindow(TSharedPtr<FGenericWindow> InNativeWindow) { NativeWindow = InNativeWindow; }
	


private:
	TSharedPtr<FGenericWindow> NativeWindow;




};