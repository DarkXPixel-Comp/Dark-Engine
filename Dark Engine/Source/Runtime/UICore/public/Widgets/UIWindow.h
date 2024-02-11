#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "Memory/TUniquePtr.h"
#include "CoreMinimal.h"


class UIWindow
{
public:
	UIWindow() {}

public:


	TSharedPtr<FGenericWindow> GetNativeWindow() { return NativeWindow; }
	void SetNativeWindow(TSharedPtr<FGenericWindow> InNativeWindow) { NativeWindow = InNativeWindow; }

	void ShowWindow() { NativeWindow->Show(); }

	FVector2f GetInitSizeInScreen() const { return InitSizeScreen; }
	FVector2f GetInitPositionInScreen() const { return InitPositionScreen; }
	bool HasWindowBorder() const { return true; }
	FString GetTitle() const { return Title; }
	


private:
	TSharedPtr<FGenericWindow> NativeWindow;
	FVector2f InitSizeScreen;
	FVector2f InitPositionScreen;
	FString Title;




};