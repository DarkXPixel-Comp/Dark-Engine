#pragma once

#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsIncludes.h"
#include "Windows/WindowsLayout.h"
#include "Containers/Array.h"


class IWindowsMessageHandler
{
public:
	virtual bool ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam) = 0;
};


class FWindowsApplication : public FGenericApplication
{
public:
	APPLICATIONCORE_API static FWindowsApplication* CreateWindowsApplication(const HINSTANCE hInstance, const HICON hIcon, const HCURSOR hCursor = NULL);
public:
	APPLICATIONCORE_API TSharedPtr<FGenericWindow> MakeWindow() override;
	APPLICATIONCORE_API virtual ~FWindowsApplication() {}

	APPLICATIONCORE_API virtual void InitializeWindow(const TSharedPtr<FGenericWindow>& InWindow, FGenericWindowDefinition InDefinition, const TSharedPtr<FGenericWindow>& InParent, bool bShow = false) override;


	APPLICATIONCORE_API virtual void AddMessageHandler(IWindowsMessageHandler& InMessageHandler);
	APPLICATIONCORE_API virtual void RemoveMessageHandler(IWindowsMessageHandler& InMessageHandler);

private:
	static LRESULT AppWndProc(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam);

	APPLICATIONCORE_API LRESULT WndProc(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam) { return 0; }

	APPLICATIONCORE_API virtual void PumpMessages() override;

	APPLICATIONCORE_API int32 ProcessMessage(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam);
	

	APPLICATIONCORE_API FWindowsApplication(const HINSTANCE hInIntance, const HICON hInIcon, const HCURSOR hCursor = NULL);


	APPLICATIONCORE_API bool Register(const HINSTANCE HInstance, const HICON HIcon);


private:
	TArray<TSharedPtr<class FWindowsWindow>> Windows;
	FPlatformLayout PlatformLayout;

	HINSTANCE HInstance = NULL;

	TArray<IWindowsMessageHandler*> MessageHandlers;
};