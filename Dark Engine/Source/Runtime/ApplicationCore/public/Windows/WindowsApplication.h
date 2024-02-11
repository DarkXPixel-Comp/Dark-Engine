#pragma once
#include "CoreTypes.h"
#include "Windows/MinimalWindowsApi.h"
#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsWindow.h"
#include "TUniquePtr.h"


#undef RegisterClass


class FWindowsApplication : public FGenericApplication
{
public:
	static FWindowsApplication* CreateWindowsApplication(const HINSTANCE HInstance, const HICON HIcon);
	static void DestroyWindowsApplication();

public:
	TSharedPtr<FGenericWindow> MakeWindow() override;
	virtual void InitializeWindow(const TSharedPtr<FGenericWindow>& InWindow,
		const TSharedPtr<FGenericWindowDefinition>& InDefinition) override;

private:
	static LRESULT AppWndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowsApplication_WndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);

	int32 ProcessMessage(HWND hWnd, uint32 Msg, WPARAM wParam, LPARAM lParam);

	FWindowsApplication(const HINSTANCE HInstance, const HICON HIcon);
	~FWindowsApplication();


	HINSTANCE InstanceHandle;

private:
	static bool RegisterClass(const HINSTANCE HInstance, const HICON HIcon);



	TArray<TSharedPtr<class FWindowsWindow>> Windows;



};

