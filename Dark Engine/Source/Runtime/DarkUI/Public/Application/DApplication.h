#pragma once
#include "Platform/Platform.h"
#include "Memory/MemoryCore.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericApplication.h"


class DUIWindow;

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

	TSharedPtr<DUIWindow> AddWindow(const TSharedPtr<DUIWindow>& InWindow, bool bShow = false);

	void InitHightDPI(bool bForceEnable);

	static FDUIApplication& Get()
	{
		return *CurrentApplication;
	}

	void Tick();


	DARKUI_API TSharedPtr<DUIWindow> GetActiveModalWindow() const;
												
public:
	DARKUI_API virtual bool ShouldProcessUserInputMessages(const TSharedPtr<FGenericWindow>& PlatformWindow) const override;
	virtual EWindowZone GetWindowZoneForPoint(const TSharedPtr<FGenericWindow>& Window, const int32 X, const int32 Y) override;

protected:
	TSharedPtr<FGenericWindow> MakeWindow(const TSharedPtr<DUIWindow>& InWindow, bool bShow = false);
	void TickPlatform(float DeltaTime);

protected:
	static TSharedPtr<DUIWindow> FindWindowByPlatformWindow(const TArray<TSharedPtr<DUIWindow>>& WindowsToSearch, const TSharedPtr<FGenericWindow>& PlatformWindow);



protected:
	TSharedPtr<class FDUIRenderer> Renderer;

	static DARKUI_API TSharedPtr<FDUIApplication> CurrentApplication;
	static DARKUI_API TSharedPtr<FGenericApplication>	PlatformApplication;

	TArray<TSharedPtr<DUIWindow>> Windows;
	TArray<TSharedPtr<DUIWindow>> VirtualWindows;
	TArray<TSharedPtr<DUIWindow>> ActiveModalWindows;

	//FDisplayMetrics DisplayMetrics;





};