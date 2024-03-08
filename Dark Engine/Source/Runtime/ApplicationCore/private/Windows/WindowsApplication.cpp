#include "Windows/WindowsApplication.h"
#include "CoreMinimal.h"

#include "Windows/WindowsWindow.h"
#include <CoreGlobals.h>


FWindowsApplication* WindowsApplication;


FWindowsApplication* FWindowsApplication::CreateWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle)
{
	WindowsApplication = new FWindowsApplication(HInstance, IconHandle);
	return WindowsApplication;
}

void FWindowsApplication::DestroyWindowsApplication()
{
	delete WindowsApplication;
	WindowsApplication = nullptr;
}

LRESULT FWindowsApplication::AppWndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	return WindowsApplication->ProcessMessage(hwnd, msg, wParam, lParam);
}

static TSharedPtr< FWindowsWindow > FindWindowByHWND(const TArray< TSharedPtr< FWindowsWindow > >& WindowsToSearch, HWND HandleToFind)
{
	for (int32 WindowIndex = 0; WindowIndex < WindowsToSearch.Num(); ++WindowIndex)
	{
		TSharedPtr< FWindowsWindow > Window = WindowsToSearch[WindowIndex];
		if (Window->GetHWnd() == HandleToFind)
		{
			return Window;
		}
	}

	return TSharedPtr< FWindowsWindow >(nullptr);
}



int32 FWindowsApplication::ProcessMessage(HWND hWnd, uint32 Msg, WPARAM wParam, LPARAM lParam)
{
	//TSharedPtr<FWindowsWindow> CurrentWindow = MakeShareble<FWindowsWindow>(reinterpret_cast<FWindowsWindow*>
	//	( GetWindowLongPtr(hWnd, GWLP_USERDATA)));

	TSharedPtr<FWindowsWindow> CurrentWindow = FindWindowByHWND(Windows, hWnd);

	if (Windows.Num() && CurrentWindow)
	{
		for (auto Handler : MessageHandlers)
		{
			Handler->ProcessMessage(hWnd, Msg, wParam, lParam);

		}
		switch (Msg)
		{
		case WM_DESTROY:
			Windows.Remove(CurrentWindow);
			if (Windows.Num() <= 0)
				GIsRequestingExit = true;
			return 0;
			break;
		case WM_QUIT:
			GIsRequestingExit = true;
			return 0;


		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, 1920, 1080);
			EndPaint(hWnd, &ps);
			return 0;
		}

		case WM_SIZE:
		{
			if (CurrentWindow)
			{
				const int32 NewWidth = (int)(short)(LOWORD(lParam));
				const int32 NewHeight = (int)(short)(HIWORD(lParam));
				if (!CurrentWindow->GetDefinition().bHasWindowBorder)
				{
					CurrentWindow->AdjustWindowRegion(NewWidth, NewHeight);
				}

				const bool bWasMinimized = (wParam == SIZE_MAXIMIZED);
			}
			break;
		}
		//case WM_SIZING:
		case WM_NCCALCSIZE:
			if (wParam && !CurrentWindow->GetDefinition().bHasWindowBorder)
			{
				if (CurrentWindow->IsMaximized())
				{
					WINDOWINFO WindowInfo;
					WindowInfo.cbSize = sizeof(WindowInfo);
					::GetWindowInfo(hWnd, &WindowInfo);

					// A pointer to the window size data that Windows will use is passed to us in lParam
					LPNCCALCSIZE_PARAMS ResizingRects = (LPNCCALCSIZE_PARAMS)lParam;
					// The first rectangle contains the client rectangle of the resized window. Decrease window size on all sides by
					// the border size.
					ResizingRects->rgrc[0].left += WindowInfo.cxWindowBorders;
					ResizingRects->rgrc[0].top += WindowInfo.cxWindowBorders;
					ResizingRects->rgrc[0].right -= WindowInfo.cxWindowBorders;
					ResizingRects->rgrc[0].bottom -= WindowInfo.cxWindowBorders;
					// The second rectangle contains the destination rectangle for the content currently displayed in the window's
					// client rect. Windows will blit the previous client content into this new location to simulate the move of
					// the window until the window can repaint itself. This should also be adjusted to our new window size.
					ResizingRects->rgrc[1].left = ResizingRects->rgrc[0].left;
					ResizingRects->rgrc[1].top = ResizingRects->rgrc[0].top;
					ResizingRects->rgrc[1].right = ResizingRects->rgrc[0].right;
					ResizingRects->rgrc[1].bottom = ResizingRects->rgrc[0].bottom;
					// A third rectangle is passed in that contains the source rectangle (client area from window pre-maximize).
					// It's value should not be changed.

					// The new window position. Pull in the window on all sides by the width of the window border so that the
					// window fits entirely on screen. We'll draw over these borders with game content.
					ResizingRects->lppos->x += WindowInfo.cxWindowBorders;
					ResizingRects->lppos->y += WindowInfo.cxWindowBorders;
					ResizingRects->lppos->cx -= 2 * WindowInfo.cxWindowBorders;
					ResizingRects->lppos->cy -= 2 * WindowInfo.cxWindowBorders;

					// Informs Windows to use the values as we altered them.
					return WVR_VALIDRECTS;
				}
				else if (wParam)
				{
					return 0;
				}
				return 0;
			}
			break;
		default:
			break;
		}

	}
	




	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void FWindowsApplication::PumpMessages()
{
	MSG Message;

	while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}


}


FWindowsApplication::FWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle) :
	InstanceHandle(HInstance)
{
	bool bClassRegister = RegisterClass(InstanceHandle, IconHandle);

	// For Drag and Drop
	OleInitialize(NULL);





}




void FWindowsApplication::InitializeWindow(const TSharedPtr<FGenericWindow>& InWindow,
	const TSharedPtr<FGenericWindowDefinition>& InDefinition)
{

	const TSharedPtr<FWindowsWindow> Window = StaticCastSharedPtr<FWindowsWindow>(InWindow);

	Windows.Add(Window);
	Window->Initialize(this, *InDefinition, InstanceHandle);

}


FWindowsApplication::~FWindowsApplication()
{
	OleUninitialize();
}


TSharedPtr<FGenericWindow> FWindowsApplication::MakeWindow()
{
	return FWindowsWindow::Make();
}


bool FWindowsApplication::RegisterClass(const HINSTANCE HInstance, const HICON HIcon)
{
	WNDCLASS WC;
	FMemory::Memzero(&WC, sizeof(WC));
	WC.style = CS_DBLCLKS;
	WC.lpfnWndProc = AppWndProc;
	WC.cbClsExtra = 0;
	WC.cbWndExtra = 0;
	WC.hInstance = HInstance;
	WC.hIcon = HIcon;
	WC.hCursor = NULL;
	WC.hbrBackground = NULL;
	WC.lpszMenuName = NULL;
	WC.lpszClassName = FWindowsWindow::AppWindowClass;

	if (!RegisterClassW(&WC))
	{
		MessageBox(NULL, TEXT("Window Registation FAILED!"), TEXT("ERROR!"), MB_ICONEXCLAMATION | MB_OK);

		return false;
	}




	return true;
}