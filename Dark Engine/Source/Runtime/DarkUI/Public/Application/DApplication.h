#pragma once
#include "Platform/Platform.h"
#include "Memory/MemoryCore.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "GenericPlatform/GenericWindow.h"


class FDUIApplication : public FGenericApplicationMessageHandler
{
	friend class DUIWidget;
public:
	DARKUI_API FDUIApplication();
	virtual ~FDUIApplication() {}

	virtual bool IsActive() const;

	TSharedPtr<class DUIWindow> AddWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow = false);

	void InitHightDPI(bool bForceEnable);

protected:
	TSharedPtr<FGenericWindow> MakeWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow = false);


protected:
	TSharedPtr<class FDUIRenderer> Renderer;

	static DARKUI_API TSharedPtr<FDUIApplication> CurrentBaseApplication;
	static DARKUI_API TSharedPtr<class FGenericApplication>	PlatformApplication;

	//FDisplayMetrics DisplayMetrics;




};