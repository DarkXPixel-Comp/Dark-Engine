#pragma once
#include "Platform/Platform.h"
#include "Memory/MemoryCore.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericApplication.h"
#include "Containers/Array.h"



class FDUIUser;
class DUIWindow;

class FDUIApplication : public FGenericApplicationMessageHandler
{
	friend class DUIWidget;

protected:
	static DARKUI_API TSharedPtr<FDUIApplication> Create(const TSharedPtr<FGenericApplication>& InPlatformApplication);

public:
	static DARKUI_API void Create();
	static DARKUI_API void Shutdown();

public:
	DARKUI_API FDUIApplication();
	virtual ~FDUIApplication() {}

	virtual bool IsActive() const { return false; }

	DARKUI_API TSharedPtr<DUIWindow> AddWindow(const TSharedPtr<DUIWindow>& InWindow, bool bShow = true);

	DARKUI_API static void InitHightDPI(bool bForceEnable);

	static FDUIApplication& Get()
	{
		return *CurrentApplication;
	}

	DARKUI_API void Tick();


	DARKUI_API TSharedPtr<DUIWindow> GetActiveModalWindow() const;

	DARKUI_API float GetApplicationScale() const;

	DARKUI_API void LocateWidgetInWindow(const FVector2d& MousePosition, const TSharedPtr<DUIWindow>& Window, bool bIgnoreEnabledStatus, int32 UserIndex) const;

	DARKUI_API bool IsWindowHousingInteractiveTooltip(const TSharedPtr<const DUIWindow>& WindowToTest) const;

	DARKUI_API void GetDisplayMetrics(FDisplayMetrics& OutMetrics) const;

	DARKUI_API void GetCachedDisplayMetrics(FDisplayMetrics& OutMetrics) const;
												
public:
	DARKUI_API virtual bool ShouldProcessUserInputMessages(const TSharedPtr<FGenericWindow>& PlatformWindow) const override;
	DARKUI_API virtual EWindowZone GetWindowZoneForPoint(const TSharedPtr<FGenericWindow>& Window, const int32 X, const int32 Y) override;
	DARKUI_API virtual void OnWindowClose(const TSharedPtr<FGenericWindow>& PlatformWindow) override;

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

	TArray<TSharedPtr<FDUIUser>> Users;

	float ApplicationScale;

	mutable FDisplayMetrics CachedDisplayMetrics;
};