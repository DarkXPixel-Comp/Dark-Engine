#include "../../public/Windows/WindowsWindow.h"
#include <windowsx.h>
#include <assert.h>

#include <Keyboard.h>
#include <Mouse.h>


uint64_t countWindows = 0;

bool is_appQuit = false;

bool is_Destroyed = false;

void (*renderFunc)() =	nullptr;
void (*resizeFunc)() =	nullptr;
void (*controlFunc)(int) = nullptr;
void (*mouseFunc)(float, float) = nullptr;

void ctlFunc(int button);
void MouseFunc(float x, float y);



#define to_str(x) std::to_string(x)


struct wndProcStruct
{
	std::string textForDrawOnScreen;
} wndProcStruct;




LRESULT CALLBACK mWndProc
(
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{	

	HDC hdc;             
	PAINTSTRUCT ps;   

	static float x = 0, y = 0;

	static bool showCurs = true;

	switch (message)
	{
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_CLOSE:
		--countWindows;
		if (countWindows == 0)
		{
			is_appQuit = true;
		}
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		is_Destroyed = true;
		break;
	case WM_SIZE:
		break;
	case WM_PAINT:
		/*hdc = BeginPaint(hWnd, &ps);
		TextOutA(hdc, 10, 20, "Hello", 7);
		EndPaint(hWnd, &ps);*/
		return 0;
		

		break;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void WindowsWindow::DrawTextOnWindow(std::string str)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	rect.left = 10;
	rect.top =	 10;
	rect.right = 200;
	rect.bottom = 200;


	hdc = BeginPaint(hWnd, &ps);

	DrawTextA(hdc, str.c_str(), str.size(), &rect, DT_NOCLIP);

	

	EndPaint(hWnd, &ps);


	wndProcStruct.textForDrawOnScreen = str;
}

void ctlFunc(int button)
{
	if (controlFunc)
		controlFunc(button);
}

void MouseFunc(float x, float y)
{
	if (mouseFunc)
		mouseFunc(x, y);
}



void WindowsWindow::SetResolution(UINT w, UINT h)
{
	width = w == 0 ? width : w;
	height = h == 0 ? height : h;


	SetWindowPos(hWnd, hWnd, leftX, topY, width, height, SWP_NOZORDER);

	Update();
}


void WindowsWindow::SetPos(UINT x, UINT y)
{
	leftX = x;
	topY = y;

	SetWindowPos(hWnd, hWnd, leftX, topY, width, height, SWP_NOZORDER);

	Update();
}

void WindowsWindow::SetResPos(UINT w, UINT h, UINT x, UINT y)
{
	width = w;
	height = h;

	leftX = x;
	topY = y;


	SetWindowPos(hWnd, hWnd, leftX, topY, width, height, SWP_NOZORDER);

	Update();
}


void WindowsWindow::SetControlFunction(void(*func)(int))
{
	controlFunc = func;
}


void abra(UINT, UINT) {}


void WindowsWindow::SetRenderFunction(void(*func)())
{
	renderFunc = func;
}

void WindowsWindow::SetResizeFunction(void(*func)())
{
	resizeFunc = func;
}


void WindowsWindow::SetMouseFunction(void(*func)(float, float))
{
	mouseFunc = func;
}


HWND WindowsWindow::GetHandle()
{
	return hWnd;
}

bool WindowsWindow::isAppQuit()
{
	return isDestroyed;
}


UINT WindowsWindow::GetWitdh()
{
	return width;
}

UINT WindowsWindow::GetHeight()
{
	return height;
}


void WindowsWindow::CloseWindow()
{
	--countWindows;
	if (countWindows == 0)
	{
		is_appQuit = true;
	}
	DestroyWindow(hWnd);
}


void WindowsWindow::Initialize(HINSTANCE hInstance, uint32_t cmdShow, LPCWSTR WindowName, UINT x, UINT y, UINT w, UINT h)
{
	leftX =	 leftX == 0 ? x : leftX;
	topY =	 topY == 0 ? y : topY;
	width =  width == 0 ? w : width;
	height = height == 0 ? h : height;

	WNDCLASSEX wSex;

	wSex.cbSize = sizeof(WNDCLASSEX);
	wSex.style = CS_HREDRAW | CS_VREDRAW;
	wSex.lpfnWndProc = mWndProc;
	wSex.cbClsExtra = 0;
	wSex.cbWndExtra = 0;
	wSex.hInstance = hInstance;
	wSex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wSex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wSex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wSex.lpszMenuName = NULL;
	wSex.lpszClassName = WindowName;
	wSex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);


	if (!RegisterClassExW(&wSex))
	{
		Logger::log(L"Error register Class" + std::wstring(WindowName), LOGGER_ERROR);

		Logger::wait();

		abort();

	}



	hWnd = CreateWindowW(WindowName,
		WindowName,
		WS_OVERLAPPEDWINDOW,
		leftX, topY,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		Logger::log(L"Error create window" + std::wstring(WindowName), LOGGER_ERROR);

		MessageBox(NULL, L"AAAAAAAAA, HEEEEEEEELP NOW WORK", L"AAAAAAAAAAAAA", NULL);

		Logger::wait();

		abort();
	}

	isDestroyed = false;

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	UpdateWindow(hWnd);

	++countWindows;

	Logger::log("Succesfully create window", LOGGER_INFO);
	Logger::log("leftX = " + to_str(leftX) + ", topY = " + to_str(topY) + ", width = " + to_str(width) + ", height = " + to_str(height), LOGGER_INFO);


	//Logger::log("Id = " + to_str(id_Object), LOGGER_INFO);

}








WindowsWindow::WindowsWindow()
{


}


void WindowsWindow::Update()
{
	if (isDestroyed)
		return;

	GetMessage(&msg, hWnd, 0, 0);
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}


	RECT rect; POINT p;

	GetWindowRect(hWnd, &rect);

	leftX = rect.left;
	topY =	rect.top;


	if (!(width == rect.right - rect.left || height == rect.bottom - rect.top))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		//resizeFunc();
	}



	if (!IsWindow(hWnd))
	{
		isDestroyed = true;

		countWindows -= 1;

		Logger::log("Window has been Destroyed. ID - ", LOGGER_INFO);
	}



	//wndProcStruct = {};

}


void WindowsWindow::Show()
{
	ShowWindow(hWnd, SW_SHOW);
}



void WindowsWindow::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}

void WindowsWindow::Maximaze()
{
	ShowWindow(hWnd, SW_MAXIMIZE);
}



void WindowsWindow::Minimize()
{
	ShowWindow(hWnd, SW_MINIMIZE);
}

void FWindowsWindow::Create(UINT w, UINT h, UINT x, UINT y)
{
	WNDCLASSEX wSex;

	wSex.cbSize = sizeof(WNDCLASSEX);
	wSex.style = CS_HREDRAW | CS_VREDRAW;
	wSex.lpfnWndProc = nullptr;
	wSex.cbClsExtra = 0;
	wSex.cbWndExtra = 0;
	wSex.hInstance = Application::GetInstance();
	wSex.hIcon = PerEngineSettings::MainIcon();
	wSex.hCursor = LoadCursor(wSex.hInstance, IDI_APPLICATION);
	wSex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wSex.lpszMenuName = NULL;
	wSex.lpszClassName = PerEngineSettings::ProjectName();
	wSex.hIconSm = PerEngineSettings::MainIcon();

	if (!RegisterClassExW(&wSex))
	{
		Logger::log(L"Error register Class", LOGGER_ERROR);
		Logger::wait();
		abort();

	}

	hWnd = CreateWindowW(PerEngineSettings::ProjectName(),
		PerEngineSettings::ProjectName(),
		WS_OVERLAPPEDWINDOW,
		x, y, w, h,
		NULL,
		NULL,
		Application::GetInstance(),
		NULL);
	


}
