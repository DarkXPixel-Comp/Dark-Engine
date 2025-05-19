#pragma once
#include "Platform/Platform.h"
#include "Memory/MemoryCore.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericApplication.h"


class FDUIApplication : public FGenericApplicationMessageHandler
{
	friend class DUIWidget;

protected:
	static DARKUI_API TSharedPtr<FDUIApplication> Create(const TSharedPtr<FGenericApplication>& InPlatformApplication);

public:
	static DARKUI_API void Create();

public:
	DARKUI_API FDUIApplication();
	virtual ~FDUIApplication() {}

	virtual bool IsActive() const { return false; }

	TSharedPtr<class DUIWindow> AddWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow = false);

	void InitHightDPI(bool bForceEnable);

	static FDUIApplication& Get()
	{
		return *CurrentApplication;
	}

	void Tick();



protected:
	TSharedPtr<FGenericWindow> MakeWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow = false);
	void TickPlatform(float DeltaTime);



protected:
	TSharedPtr<class FDUIRenderer> Renderer;

	static DARKUI_API TSharedPtr<FDUIApplication> CurrentApplication;
	static DARKUI_API TSharedPtr<class FGenericApplication>	PlatformApplication;

	//FDisplayMetrics DisplayMetrics;





};