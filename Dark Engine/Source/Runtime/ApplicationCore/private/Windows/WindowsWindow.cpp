#include "Windows/WindowsWindow.h"
#include <Application/Application.h>
#include <Logging/Logger.h>
#include <CoreDefines.h>
#include <InputCore.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "CoreGlobals.h"
#include "dwmapi.h"
#include <HAL/DarkMemory.h>


const TCHAR FWindowsWindow::AppWindowClass[] = TEXT("DarkWindow");



void FWindowsWindow::AdjustWindowRegion(int32 Width, int32 Height)
{
	RegionWidth = Width;
	RegionHeight = Height;

	HRGN Region = MakeWindowRegionObject(true);

	verify(SetWindowRgn(HWnd, Region, false));


}

int32 FWindowsWindow::GetWindowBorderSize()	const
{
	if (!WndDefinition.bHasWindowBorder)
		return 0;

	WINDOWINFO WindowInfo;
	WindowInfo.cbSize = sizeof(WindowInfo);
	GetWindowInfo(HWnd, &WindowInfo);

	return WindowInfo.cxWindowBorders;
}

HRGN FWindowsWindow::MakeWindowRegionObject(bool bIncludeBorderWhenMaximized) const
{
	HRGN Region;

	if (RegionWidth != -1 && RegionHeight != -1)
	{
		const bool bIsBorderlessGameWindow = !WndDefinition.bHasWindowBorder;
		if (IsMaximized())
		{
			if (bIsBorderlessGameWindow)
			{
				WINDOWINFO WindowInfo;
				FMemory::Memzero(&WindowInfo, sizeof(WINDOWINFO));
				WindowInfo.cbSize = sizeof(WindowInfo);
				GetWindowInfo(HWnd, &WindowInfo);

				const int32 WindowBorderSize = bIncludeBorderWhenMaximized ? WindowInfo.cxWindowBorders : 0;
				Region = CreateRectRgn(WindowBorderSize, WindowBorderSize,
					RegionWidth + WindowBorderSize, RegionHeight + WindowBorderSize);
			}
			else
			{
				const int32 WindowBorderSize = bIncludeBorderWhenMaximized ? GetWindowBorderSize() : 0;
				Region = CreateRectRgn(WindowBorderSize, WindowBorderSize,
					RegionWidth - WindowBorderSize, RegionHeight - WindowBorderSize);
			}
		}
		else
		{
			const bool bUseCornerRadius = bIsBorderlessGameWindow && WndDefinition.CornerRadius > 0;

			if (bUseCornerRadius)
			{
				Region = CreateRoundRectRgn(0, 0, RegionWidth + 1, RegionHeight + 1, 
					WndDefinition.CornerRadius, WndDefinition.CornerRadius);
			}
			else
			{
				Region = CreateRectRgn(0, 0, RegionWidth, RegionHeight);
			}
		}

	}
	else
	{
		RECT rcWnd;
		GetWindowRect(HWnd, &rcWnd);
		Region = CreateRectRgn(0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);
	}

	return Region;


}


FIntPoint FWindowsWindow::GetRelativeMousePos() const
{
	POINT Result;
	GetCursorPos(&Result);
	ScreenToClient(HWnd, &Result);


	return FIntPoint(Result.x, Result.y);
}

void FWindowsWindow::ReshapeWindow(int32 NewX, int32 NewY, int32 NewWidth, int32 NewHeight)
{
	WINDOWINFO WindowInfo;
	WindowInfo.cbSize = sizeof(WindowInfo);
	::GetWindowInfo(HWnd, &WindowInfo);

	AspectRatio = (float)NewWidth / (float)NewHeight;

	// Don't resize the window if it's been embedded inside another application, the
	// outer application will take care of it.
	//if (WindowMode == EWindowMode::Windowed)
	//{
	//	RegionWidth = VirtualWidth = NewWidth;
	//	RegionHeight = VirtualHeight = NewHeight;
	//	return;
	//}

	// X,Y, Width, Height defines the top-left pixel of the client area on the screen
	if (WndDefinition.bHasWindowBorder)
	{
		// This adjusts a zero rect to give us the size of the border
		RECT BorderRect = { 0, 0, 0, 0 };
		::AdjustWindowRectEx(&BorderRect, WindowInfo.dwStyle, false, WindowInfo.dwExStyle);

		// Border rect size is negative - see MoveWindowTo
		NewX += BorderRect.left;
		NewY += BorderRect.top;

		// Inflate the window size by the OS border
		NewWidth += BorderRect.right - BorderRect.left;
		NewHeight += BorderRect.bottom - BorderRect.top;
	}

	// the window position is the requested position
	int32 WindowX = NewX;
	int32 WindowY = NewY;

	// If the window size changes often, only grow the window, never shrink it
	const bool bVirtualSizeChanged = NewWidth != VirtualWidth || NewHeight != VirtualHeight;
	VirtualWidth = NewWidth;
	VirtualHeight = NewHeight;

	//if (Definition->SizeWillChangeOften)
	//{
	//	// When SizeWillChangeOften is set, we set a minimum width and height window size that we'll keep allocated
	//	// even when the requested actual window size is smaller.  This just avoids constantly resizing the window
	//	// and associated GPU buffers, which can be very slow on some platforms.

	//	const RECT OldWindowRect = WindowInfo.rcWindow;
	//	const int32 OldWidth = OldWindowRect.right - OldWindowRect.left;
	//	const int32 OldHeight = OldWindowRect.bottom - OldWindowRect.top;

	//	const int32 MinRetainedWidth = Definition->ExpectedMaxWidth != INDEX_NONE ? Definition->ExpectedMaxWidth : OldWidth;
	//	const int32 MinRetainedHeight = Definition->ExpectedMaxHeight != INDEX_NONE ? Definition->ExpectedMaxHeight : OldHeight;

	//	NewWidth = FMath::Max(NewWidth, FMath::Min(OldWidth, MinRetainedWidth));
	//	NewHeight = FMath::Max(NewHeight, FMath::Min(OldHeight, MinRetainedHeight));
	//}

	if (IsMaximized())
	{
		Restore();
	}

	// We use SWP_NOSENDCHANGING when in fullscreen mode to prevent Windows limiting our window size to the current resolution, as that 
	// prevents us being able to change to a higher resolution while in fullscreen mode
	::SetWindowPos(HWnd, nullptr, WindowX, WindowY, NewWidth, NewHeight, SWP_NOZORDER | SWP_NOACTIVATE | ((WindowMode == EWindowMode::Fullscreen) ? SWP_NOSENDCHANGING : 0));

	bool bAdjustSizeChange = bVirtualSizeChanged;
	bool bAdjustCorners = WndDefinition.CornerRadius > 0;

	if (!WndDefinition.bHasWindowBorder && (bAdjustSizeChange || bAdjustCorners))
	{
		AdjustWindowRegion(VirtualWidth, VirtualHeight);
	}

}

#ifdef IMGUI
void FWindowsWindow::InitImGui()
{
	ImGui_ImplWin32_Init(HWnd);
}
#endif

void FWindowsWindow::BeginImGui()
{
#ifdef  IMGUI
	ImGui_ImplWin32_NewFrame();
#endif //  IMGUI

}

void FWindowsWindow::Initialize(FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition, HINSTANCE InHInstance)
{
//Definition = std::make_shared<FGenericWindowDefinition>(InDefinition);
	WndDefinition = InDefinition;
	OwningApplication = Application;

	uint32 WindowStyle = 0;
	uint32 WindowExStyle = 0;

	const float XInitialRect = InDefinition.XPositionOnScreen;
	const float YInitialRect = InDefinition.YPositionOnScreen;

	const float WidthInitial = InDefinition.WidthOnScreen;
	const float HeightInitial = InDefinition.HeightOnScreen;

	int32 ClientX = FMath::TruncToInt(XInitialRect);
	int32 ClientY = FMath::TruncToInt(YInitialRect);
	int32 ClientWidth = FMath::TruncToInt(WidthInitial);
	int32 ClientHeight = FMath::TruncToInt(HeightInitial);

	int32 WindowX = ClientX;
	int32 WindowY = ClientY;
	int32 WindowWidth = ClientWidth;
	int32 WindowHeight = ClientHeight;

	if (InDefinition.bHasWindowBorder)
	{
		WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
		//WindowStyle = WS_POPUP | WS_BORDER;
		WindowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_LAYERED | WS_EX_TOPMOST;

		RECT BorderRect = { 0, 0, 0, 0 };

		AdjustWindowRectEx(&BorderRect, WindowStyle, false, WindowExStyle);

		WindowX += BorderRect.left;
		WindowY += BorderRect.top;
		WindowWidth += BorderRect.right - BorderRect.left;
		WindowHeight += BorderRect.bottom - BorderRect.top;


	}
	else
	{
		WindowExStyle = WS_EX_WINDOWEDGE | WS_EX_APPWINDOW;

		WindowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		//WindowExStyle |= WS_EX_APPWINDOW;

	}


	FString Title = InDefinition.Title; // wstring(Definition->Title.begin(), Definition->Title.end()).data();


	HWnd = CreateWindowEx(WindowExStyle, AppWindowClass, *Title, WindowStyle, WindowX,
		WindowY, WindowWidth, WindowHeight, NULL, NULL, InHInstance, NULL);

	WindowMode = EWindowMode::Windowed;

	if (HWnd == NULL)
	{
		const uint32 Error = GetLastError();
		return;
	}

	VirtualWidth = ClientWidth;
	VirtualHeight = ClientHeight;

	ReshapeWindow(ClientX, ClientY, ClientWidth, ClientHeight);

	SetWindowText(HWnd, *Title);
	if (!InDefinition.bHasWindowBorder)
	{
		const DWMNCRENDERINGPOLICY RenderingPolicy = DWMNCRP_DISABLED;
		verify(SUCCEEDED(DwmSetWindowAttribute(HWnd, DWMWA_NCRENDERING_POLICY, &RenderingPolicy, sizeof(RenderingPolicy))));

		const BOOL bEnableNCPaint = false;
		verify(SUCCEEDED(DwmSetWindowAttribute(HWnd, DWMWA_ALLOW_NCPAINT, &bEnableNCPaint, sizeof(bEnableNCPaint))));


		WindowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

		verify(SetWindowLong(HWnd, GWL_STYLE, WindowStyle));


		uint32 SetWindowPosFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;

		SetWindowPos(HWnd, nullptr, 0, 0, 0, 0, SetWindowPosFlags);

		DeleteMenu(GetSystemMenu(HWnd, false), SC_CLOSE, MF_BYCOMMAND);

		AdjustWindowRegion(ClientWidth, ClientHeight);
	}
	else
	{
		EnableMenuItem(GetSystemMenu(HWnd, false), SC_CLOSE, MF_GRAYED);
	}

	//RegisterDragDrop(HWnd, this);


#ifdef IMGUI
	ImGui_ImplWin32_Init(HWnd);
#endif

	SetWindowLongPtr(HWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}


TSharedPtr<FGenericWindow> FWindowsWindow::Make()
{
	return MakeShareble(new FWindowsWindow());
}




FWindowsWindow::FWindowsWindow(FWindowsWindowManager* maneger, UINT index)
{
	m_Manager = (maneger);
	m_Index = index;
}

void FWindowsWindow::Create(UINT w, UINT h, UINT x, UINT y, string name, FWindowsWindow* Parent)
{
	WNDCLASSEX wSex;

	wstring wClassName(name.begin(), name.end());

	LPCWSTR ClassName = wClassName.c_str();




	wSex.cbSize = sizeof(WNDCLASSEX);
	wSex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wSex.lpfnWndProc = FWindowsWindow::WndProc;
	wSex.cbClsExtra = 0;
	wSex.cbWndExtra = 0;
	wSex.hInstance = Application::GetInstance();
	wSex.hIcon = LoadIcon(Application::GetInstance(), MAKEINTRESOURCE(554));
	wSex.hCursor = LoadCursor(Application::GetInstance(), IDC_ARROW);
	wSex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wSex.lpszMenuName = NULL;
	wSex.lpszClassName = ClassName;
	wSex.hIconSm = LoadIcon(Application::GetInstance(), IDI_APPLICATION);




	if (!RegisterClassExW(&wSex))
	{
		auto er = GetLastError();

		Logger::log(FString(TEXT("Error register Class")), LOGGER_ERROR);
		Logger::wait();
		abort();

	}


	uint32 WindowStyle = 0;
	uint32 WindowExStyle = 0;


	m_Wnd = CreateWindowW(ClassName,
		ClassName,
		WS_OVERLAPPEDWINDOW,
		x, y, w, h,
		NULL,
		NULL,
		Application::GetInstance(),
		NULL);
	
	if (!m_Wnd)
	{
		Logger::log("Error create Window " + name);

		Logger::wait();

		abort();

		return;
	}


	SetWindowLongPtr(m_Wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLong(m_Wnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);


	ShowWindow(m_Wnd, Application::CmdShow());
	UpdateWindow(m_Wnd);


	RECT rect2;
	GetWindowRect(m_Wnd, &rect2);

	leftX = rect2.left;
	topY = rect2.top;
	width = rect2.right - rect2.left;
	height = rect2.bottom - rect2.top;

	m_Close = false;


	Log("Success create window ", cstr(name), " ", icstr(width), "x", icstr(height));
}

void FWindowsWindow::Destroy()
{
	if (bIsDestroyed)
	{
		return;
	}
#ifdef  IMGUI
	ImGui_ImplWin32_Shutdown();
#endif //  IMGUI

	bIsDestroyed = true;
}

void FWindowsWindow::Tick(float DeltaTime)
{
#ifdef  IMGUI
	ImGui_ImplWin32_NewFrame();
#endif //  IMGUI
}

void FWindowsWindow::Update()
{
}

void FWindowsWindow::SetResolution(UINT w, UINT h)
{
	width = w;
	height = h;

	SetWindowPos(m_Wnd, NULL, leftX, topY, w, h, NULL);

}

void FWindowsWindow::SetPos(UINT x, UINT y)
{
	leftX = x;
	topY = y;

	SetWindowPos(m_Wnd, NULL, x, y, width, height, NULL);

}

void FWindowsWindow::SetResPos(UINT w, UINT h, UINT x, UINT y)
{
	leftX = x;
	topY = y;
	width = w;
	height = h;

	SetWindowPos(m_Wnd, NULL, x, y, w, h, NULL);
}

void FWindowsWindow::SetWindowTitle(FString str)
{
	SetWindowText(m_Wnd, strw(str).c_str());
}

void FWindowsWindow::SetTitle(const TCHAR* const Title)
{
	SetWindowText(HWnd, Title);
}

bool FWindowsWindow::isClose()
{
	return m_Close;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT FWindowsWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	FWindowsWindow* pThis = reinterpret_cast<FWindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	FInputCore::InputProc(hwnd, msg, wParam, lParam);

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return 1;
	}

	switch (msg)
	{
	case WM_CREATE:
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return DefWindowProcW(hwnd, msg, wParam, lParam);
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return DefWindowProcW(hwnd, msg, wParam, lParam);
		break;
	}

	case WM_CLOSE:
		pThis->Destroy();
		break;

	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		pThis->onResizeWindow.BroadCast(width, height);
		break;
	}

	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

FWindowsWindow* FWindowsWindowManager::CreateWindow(UINT Weight, UINT Height, FString Name)
{
	FWindowsWindow* window = new FWindowsWindow(this, (UINT)windows.GetSize());

	window->Create(Weight, Height, 0, 0, Name.ToString() + to_string(windows.GetSize()), NULL);

	windows.Add(window);


	return window;

}

void FWindowsWindowManager::Update()
{
	for (auto& i : windows)
	{
		i->Update();
	}




	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			GIsRequestingExit = true;
			return;
		}


		TranslateMessage(&msg);
		DispatchMessageW(&msg);

	}

	//if (timerIsStarted)
	//{
	//	//WaitForSingleObject(&hTimer, INFINITE);
	//	WaitForMultipleObjects(1, &hTimer, TRUE, INFINITE);
	//}

	//MsgWaitForMultipleObjects(1, &hTimer, TRUE, INFINITE, QS_ALLEVENTS);

}


void FWindowsWindowManager::SetDelay(int ms)
{
	//KillTimer(NULL, (UINT_PTR)hTimer);
	delay = ms;

	if (hTimer)
	{
		SetWaitableTimer(hTimer, &li, delay, NULL, NULL, FALSE);
		timerIsStarted = true;
	}
}



FWindowsWindowManager::FWindowsWindowManager()
{
	hTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	li.QuadPart = 0;

	delay = 1;


	if (hTimer)
	{
		//SetWaitableTimer(hTimer, &li, delay, NULL, NULL, FALSE);
	}

}

FWindowsWindow::~FWindowsWindow()
{

	this->Destroy();


}


void FWindowsWindowManager::Destroy(UINT index)
{
	auto wnd = windows[index];

	wnd->Destroy();


	delete windows[index];

	windows.Erase(windows.begin() + index);
}

void FWindowsWindowManager::Quit()
{
	for (size_t i = 0; i < windows.GetSize(); i++)
	{
		delete windows[i];
	}


	windows.Empty();
	KillTimer(NULL, (UINT_PTR)hTimer);
	timerIsStarted = false;

}


FWindowsWindowManager::~FWindowsWindowManager()
{
	Quit();

}


HRESULT FWindowsWindow::DragEnter(IDataObject* DataObject, DWORD KewState, POINTL CursorPosition, DWORD* CursorEffect)
{
	return S_OK;
}

HRESULT FWindowsWindow::DragOver(DWORD KeyState, POINTL CursorPosition, DWORD* CursorEffect)
{
	return S_OK;
}

HRESULT	FWindowsWindow::DragLeave()
{
	return S_OK;
}


HRESULT FWindowsWindow::Drop(IDataObject* DataObject, DWORD KeyState, POINTL CursorPosition, DWORD* CursorEffect)
{
	return S_OK;
}


HRESULT FWindowsWindow::QueryInterface(REFIID iid, void** ppvObject)
{
	return S_OK;
}

ULONG FWindowsWindow::AddRef()
{
	return S_OK;
}

ULONG FWindowsWindow::Release()
{
	return S_OK;
}



