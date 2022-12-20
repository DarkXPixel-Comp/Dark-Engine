#pragma once

#include <Source/Runtime/Core/CoreMinimal.h>

#include <string>

#include <d3d12.h>

#include <functional>

#undef CreateWindow;



class FWindowsWindow;


class FWindowsWindowManager
{

	void CreateWindow(UINT Weight, UINT Height, std::string Name);



private:
	std::vector<FWindowsWindow*> windows;


};




class FWindowsWindow
{
	void Create(UINT w, UINT h, UINT x, UINT y);
	void Destroy();







private:
	UINT width{ 0 };
	UINT height{ 0 };
	UINT leftX{ 0 };
	UINT topY{ 0 };

	HWND	hWnd;
	MSG		msg;

	D3D12_VIEWPORT Viewport;
	D3D12_RECT Rect;







};



class WindowsWindow 
{
public:
	WindowsWindow();
	/*WindowsWindow(const WindowsWindow&) = delete;
	WindowsWindow(WindowsWindow&&) = delete;*/


	//~WindowsWindow();


	HWND GetHandle(); 

	void Initialize(HINSTANCE hInstance, uint32_t cmdShow, LPCWSTR WindowName, UINT x, UINT y, UINT w, UINT h);

	void Show();

	void Hide();

	void Maximaze();

	void Minimize();

	void Update();

	bool isAppQuit();

	void CloseWindow();

	void SetRenderFunction(void(*func)());

	//void SetRenderFunction(std::function<void());

	void SetResizeFunction(void(*func)());

	void SetResolution(UINT w, UINT h);

	void SetPos(UINT x, UINT y);

	void SetResPos(UINT w, UINT h, UINT x, UINT y);


	UINT GetWitdh();
	UINT GetHeight();

	UINT GetTopX() { return leftX; }
	UINT GetTopY() { return topY; }



	void SetControlFunction(void(*func)(int));

	void SetMouseFunction(void(*func)(float, float));

	void SetFullScreenMode(bool isFullScreen);

	void DrawTextOnWindow(std::string str);
	

private:
	UINT width	{ 0 };
	UINT height	{ 0 };
	UINT leftX{ 0 };
	UINT topY{ 0 };

	HWND	hWnd;
	MSG		msg;

	D3D12_VIEWPORT Viewport;
	D3D12_RECT Rect;

	bool isDestroyed = true;

};


