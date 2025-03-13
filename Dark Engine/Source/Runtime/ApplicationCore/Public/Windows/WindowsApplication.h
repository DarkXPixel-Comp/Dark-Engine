#pragma once

#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsIncludes.h"
#include "Windows/WindowsLayout.h"
#include "Containers/Array.h"


class APPLICATIONCORE_API FWindowsApplication : public FGenericApplication
{
public:
	static FWindowsApplication* CreateWindowsApplication(const HINSTANCE hInstance, const HICON hIcon, const HCURSOR hCursor = NULL);
public:
	TSharedPtr<FGenericWindow> MakeWindow() override { return nullptr; }
	virtual ~FWindowsApplication() {}

	virtual void InitializeWindow(const TSharedPtr<FGenericWindow>& InWindow, const TSharedPtr<FGenericWindowDefinition>& InDefinition) override {}

private:
	static LRESULT AppWndProc(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam);
	static bool RegisterApplicationClass(const HINSTANCE hInInstance, const HICON hInIcon, const HCURSOR hInCursor) { return 0; }

	LRESULT WndProc(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam) { return 0; }

	virtual void PumpMessages() override;

	int32 ProcessMessage(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam);
	

	FWindowsApplication(const HINSTANCE hInIntance, const HICON hInIcon, const HCURSOR hCursor = NULL) {}

private:
	TArray<TSharedPtr<class FWindowsWindow>> Windows;
	FPlatformLayout PlatformLayout;
};