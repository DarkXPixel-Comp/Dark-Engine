#pragma once

#include <Source/Runtime/Core/CoreMinimal.h>

#include <string>

#include <d3d12.h>

#include <functional>

#include <Core/Delegate/Delegate.h>


#include <Keyboard.h>
#include <Mouse.h>

#undef CreateWindow;

DECLARE_MULTICAST_DELEGATE_TwoParams(ResizeWindow, long, long);
DECLARE_MULTICAST_DELEGATE(FDestroyWindow);


class FWindowsWindow;

using namespace std;

class FWindowsWindowManager
{

public:
	FWindowsWindow* CreateWindow(UINT Weight, UINT Height, std::string Name);
	FWindowsWindow* GetPrimalWindow() { return windows.size() != 0 ? windows[0] : nullptr; }
	FWindowsWindow* GetWindow(UINT index) { return index > windows.size() ? nullptr : windows[index]; }
	void Update();
	void Destroy(UINT index);
	void Quit();




private:
	std::vector<FWindowsWindow*> windows;

	MSG msg;

};




class FWindowsWindow
{
	friend FWindowsWindowManager;
public:
	FWindowsWindow(FWindowsWindowManager* manager, UINT index);
	void Create(UINT w, UINT h, UINT x, UINT y, string name, FWindowsWindow* Parent);
	void Destroy();
	void Update();

	void Show() { ShowWindow(m_Wnd, SW_SHOW); }

	void Hide() { ShowWindow(m_Wnd, SW_HIDE); }

	void Maximaze() { ShowWindow(m_Wnd, SW_MAXIMIZE); }

	void Minimize() { ShowWindow(m_Wnd, SW_MINIMIZE); }

	void SetResolution(UINT w, UINT h);

	void SetPos(UINT x, UINT y);

	void SetResPos(UINT w, UINT h, UINT x, UINT y);

	bool isClose() { return m_Close; }

	UINT GetWitdh() { return width; }
	UINT GetHeight() { return height; }

	UINT GetIndex() { return m_Index; }

	HWND GetHandle() { return m_Wnd; }

	void SetWindowTitle(std::string str);

	int MouseX = 0, MouseY = 0;


	ResizeWindow onResizeWindow;
	FDestroyWindow onDestroyWindow;




private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);






private:
	UINT width{ 0 };
	UINT height{ 0 };
	UINT leftX{ 0 };
	UINT topY{ 0 };

	HWND	m_Wnd;
	MSG		msg;

	D3D12_VIEWPORT Viewport;
	D3D12_RECT Rect;

	string m_Name;

	FWindowsWindowManager* m_Manager;

	bool m_Close = true;

	UINT m_Index;




};


