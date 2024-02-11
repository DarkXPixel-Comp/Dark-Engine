#pragma once

#include "CoreTypes.h"
#include "Containers/DarkString.h"
#include "CoreMinimal.h"


enum EWindowMode
{
	Fullscreen,
	WindowedFullscreen,
	Windowed
};

class FGenericWindow
{
public:
	FGenericWindow() {}

	virtual bool GetFullScreenInfo(int32& X, int32& Y, int32& Width, int32& Height) { return true; }

	virtual void Destroy() {}

	virtual void Show() {}

	virtual TSharedPtr<FGenericWindow> MakeWindow() { return MakeShareble(new FGenericWindow()); }

	virtual void* GetOSWindowHandle() const { return nullptr; }

	virtual void SetTitle(FString Title) {}


protected:
	bool bIsVisible = false;
	EWindowMode WindowMode;




	



};