#pragma once

#include "CoreTypes.h"
#include "Containers/DarkString.h"
#include "CoreMinimal.h"
#include "Math/IntRect.h"


enum EWindowMode
{
	Fullscreen,
	WindowedFullscreen,
	Windowed
};


enum class ECursorArea
{
	Client,
	Close,
	Caption,
	Maximize,
	Minimize,
	Menu
};

class FGenericWindow
{
public:
	FGenericWindow() {}
	virtual ~FGenericWindow() {}

	virtual bool GetFullScreenInfo(int32& X, int32& Y, int32& Width, int32& Height) { return true; }

	virtual void Destroy() {}

	virtual void Show() {}

	virtual void Update() {}

	virtual void Maximize() {}

	virtual void Minimize() {}

	virtual TSharedPtr<FGenericWindow> MakeWindow() { return MakeShareble(new FGenericWindow()); }

	virtual void* GetOSWindowHandle() const { return nullptr; }

	virtual void SetTitle(FString Title) {}

	virtual FIntRect GetRect() const { return FIntRect(); }
	virtual FIntPoint GetSize() const { return FIntPoint(); }
	virtual FIntPoint GetPosition() const { return FIntPoint(); }

	virtual EWindowMode GetWindowMode() const { return WindowMode; }

	virtual FIntPoint GetRelativeMousePos() const { return FIntPoint(); }

	virtual bool IsMaximized() const { return false; }

	virtual void Restore() {}
						  
#ifdef IMGUI
	virtual void InitImGui() {}
	virtual void ShutdownImGui() {}
	virtual void BeginImGui() {}
#endif


	ECursorArea bTitleBarHovarered = ECursorArea::Client;

protected:
	bool bIsVisible = false;
	EWindowMode WindowMode = EWindowMode::Windowed;




	



};