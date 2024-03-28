#pragma once
#include "CoreTypes.h"
#include "Windows/MinimalWindowsApi.h"
#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsWindow.h"
#include "Memory/TUniquePtr.h"


#undef RegisterClass



class IWindowsMessageHandler
{
public:
	virtual int32 ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam) = 0;
};





class FWindowsApplication : public FGenericApplication
{
public:
	static FWindowsApplication* CreateWindowsApplication(const HINSTANCE HInstance, const HICON HIcon, const HCURSOR HCursor = NULL);
	static void DestroyWindowsApplication();

public:
	TSharedPtr<FGenericWindow> MakeWindow() override;
	virtual void InitializeWindow(const TSharedPtr<FGenericWindow>& InWindow,
		const TSharedPtr<FGenericWindowDefinition>& InDefinition) override;


	virtual void SetCapture(const TSharedPtr<FGenericWindow>& InWindow);

	virtual FIntPoint GetMousePos() const;

private:
	static LRESULT AppWndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowsApplication_WndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);

	virtual void PumpMessages() override;

	virtual void AddMessageHandler(IWindowsMessageHandler& InMessageHandler) {}


	int32 ProcessMessage(HWND hWnd, uint32 Msg, WPARAM wParam, LPARAM lParam);

	FWindowsApplication(const HINSTANCE HInstance, const HICON HIcon, const HCURSOR HCursor = NULL);
	~FWindowsApplication();


	HINSTANCE InstanceHandle;

private:
	static bool RegisterClass(const HINSTANCE HInstance, const HICON HIcon, const HCURSOR HCursor);



	TArray<TSharedPtr<class FWindowsWindow>> Windows;
	TArray<TSharedPtr<IWindowsMessageHandler>>	MessageHandlers;



};

