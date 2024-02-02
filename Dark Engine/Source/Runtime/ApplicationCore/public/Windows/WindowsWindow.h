#pragma once
#include <Core.h>
#include <Delegate/Delegate.h>
#include <Windows.h>
#include <Containers/DarkString.h>
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Windows/WindowsApplication.h"

#undef CreateWindow

DECLARE_MULTICAST_DELEGATE_TwoParams(ResizeWindow, long, long);
DECLARE_MULTICAST_DELEGATE(FDestroyWindow);
DECLARE_DELEGATE(FUpdateWindow);


class FWindowsWindow;
class FWindowsWindowManager;

using namespace std;





class DENGINE_API FWindowsWindow  : public FGenericWindow
{
	friend FWindowsWindowManager;

public:
	static const TCHAR AppWindowClass[];

public:
	FWindowsWindow(FWindowsWindowManager* manager, UINT index);
	~FWindowsWindow();
	void Create(UINT w, UINT h, UINT x, UINT y, string name, FWindowsWindow* Parent);
	void Destroy();
	void Update();


	void Initialize(FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition, HINSTANCE InHInstance, const FWindowsWindow& InParent);

public:
	virtual void* GetOSWindowHandle() const override { return HWnd; }


	void Show() { ShowWindow(m_Wnd, SW_SHOW); }

	void Hide() { ShowWindow(m_Wnd, SW_HIDE); }

	void Maximaze() { ShowWindow(m_Wnd, SW_MAXIMIZE); }

	void Minimize() { ShowWindow(m_Wnd, SW_MINIMIZE); }

	HWND GetHWnd() { return HWnd; }

	void SetWindowFocus();

	bool IsPointInWindow(int32 X, int32 Y);

	int32 GetWindowBorderSize();
	int32 GetWindowTitleBarSize();

	void SetResolution(UINT w, UINT h);

	void SetPos(UINT x, UINT y);

	void SetResPos(UINT w, UINT h, UINT x, UINT y);

	bool isClose();

	UINT GetWitdh() { return width; }
	UINT GetHeight() { return height; }

	UINT GetIndex() { return m_Index; }

	HWND GetHandle() { return m_Wnd; }

	void SetWindowTitle(FString str);

	UINT GetRefreshRate() { return 75; }

	int MouseX = 0, MouseY = 0;


	ResizeWindow onResizeWindow;
	FDestroyWindow onDestroyWindow;
	FUpdateWindow onUpdateWindow;



private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	TSharedPtr<FGenericWindowDefinition> Definition;
	FWindowsApplication* OwningApplication;





private:
	UINT width{ 0 };
	UINT height{ 0 };
	UINT leftX{ 0 };
	UINT topY{ 0 };

	HWND	m_Wnd;
	MSG		msg;


	string m_Name;

	FWindowsWindowManager* m_Manager;

	bool m_Close = true;

	UINT m_Index;





///////////////////



private:
	HWND HWnd;
	float AspectRatio;
	bool bIsVisible;


};




class DENGINE_API FWindowsWindowManager
{
	HANDLE hTimer;
	bool timerIsStarted = false;
	LARGE_INTEGER li;
	UINT_PTR timerPtr;
	long delay;

public:
	FWindowsWindowManager();
	~FWindowsWindowManager();
	FWindowsWindow* CreateWindow(UINT Weight, UINT Height, FString Name);
	//FWindowsWindow* GetPrimalWindow() { return windows.size() != 0 ? windows[0] : nullptr; }
	FWindowsWindow* GetWindow(UINT index = 0) { return index > windows.GetSize() ? nullptr : windows[index]; }
	void Update();
	void Destroy(UINT index);
	void Quit();
	void SetDelay(int ms);
	bool WindowsIsClose()
	{
		for (auto i : windows)
		{
			if (!i->isClose())
				return false;
		}

		return true;
	}




private:
	TArray<FWindowsWindow*> windows;

	MSG msg;

};
