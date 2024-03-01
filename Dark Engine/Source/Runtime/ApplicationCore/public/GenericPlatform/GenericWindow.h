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

	virtual void Update() {}

	virtual void Maximize() {}

	virtual TSharedPtr<FGenericWindow> MakeWindow() { return MakeShareble(new FGenericWindow()); }

	virtual void* GetOSWindowHandle() const { return nullptr; }

	virtual void SetTitle(FString Title) {}

	virtual FIntRect GetRect() const { return FIntRect(); }
	virtual FIntPoint GetSize() const { return FIntPoint(); }
	virtual FIntPoint GetPosition() const { return FIntPoint(); }

	virtual EWindowMode GetWindowMode() const { return WindowMode; }

#ifdef IMGUI
	virtual void InitImGui() {}
#endif


protected:
	bool bIsVisible = false;
	EWindowMode WindowMode;




	



};