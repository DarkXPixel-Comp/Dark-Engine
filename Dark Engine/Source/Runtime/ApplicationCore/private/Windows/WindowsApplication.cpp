#include "Windows/WindowsApplication.h"
#include "CoreMinimal.h"

#include "Windows/WindowsWindow.h"
#include <CoreGlobals.h>
#include "imgui_impl_win32.h"
#include "Math/MathFwd.h"
#include "windowsx.h"
#include <HAL/DarkMemory.h>

#undef IsMaximized


FWindowsApplication* WindowsApplication;


FWindowsApplication* FWindowsApplication::CreateWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle, const HCURSOR HCursor)
{
	WindowsApplication = new FWindowsApplication(HInstance, IconHandle, HCursor);
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

void FWindowsApplication::SetCapture(const TSharedPtr<FGenericWindow>& InWindow)
{
	if (InWindow.get())
	{
		::SetCapture((HWND)InWindow->GetOSWindowHandle());
	}
	else
	{
		::ReleaseCapture();
	}
}


FIntPoint FWindowsApplication::GetMousePos() const
{
	POINT Coords;
	GetCursorPos(&Coords);
	return FIntPoint(Coords.x, Coords.y);
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


POINTS Pos;

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


		if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
		{
			return true;
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

		/*case WM_PAINT:
		{
			MessageHandler->OnWindowDraw(CurrentWindow);
			return 0;
		}*/

	/*	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, 1920, 1080);
			EndPaint(hWnd, &ps);
			return 0;
		}*/


		//case WM_LBUTTONDOWN:
		//{
		//	return DefWindowProcW()
		//}

		//case WM_INPUT:
		//{
		//	uint32 Size = 0;
		//	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &Size, sizeof(RAWINPUTHEADER));

		//	TUniquePtr<uint8[]> RawData = make_unique<uint8[]>(Size);
		//	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, RawData.get(), &Size, sizeof(RAWINPUTHEADER)) == Size)
		//	{
		//		const RAWINPUT* const Raw = (const RAWINPUT* const)RawData.get();

		//		if (Raw->header.dwType == RIM_TYPEMOUSE)
		//		{
		//			const bool IsAbsoluteInput = (Raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE;
		//			if (IsAbsoluteInput)
		//			{
		//				MessageHandler->OnMouseMove();
		//			}
		//			else
		//			{
		//				const int xPosRelative = Raw->data.mouse.lLastX;
		//				const int yPosRelative = Raw->data.mouse.lLastY;

		//				MessageHandler->OnRawMouseMove(xPosRelative, yPosRelative);

		//			}
		//		}

		//	}
		//	break;
		//}

		//case WM_LBUTTONDBLCLK:
		//case WM_LBUTTONDOWN:
		//case WM_LBUTTONUP:
		//{
		//	POINT CursorPoint;
		//	CursorPoint.x = GET_X_LPARAM(lParam);
		//	CursorPoint.y = GET_Y_LPARAM(lParam);

		//	ClientToScreen(hWnd, &CursorPoint);

		//	const FVector2D CursorPos(CursorPoint.x, CursorPoint.y);
		//	EMouseButtons::Type MouseButton = EMouseButtons::Invalid;

		//	bool bDoubleClick = false;
		//	bool bMouseUp = false;

		//	switch (Msg)
		//	{
		//	case WM_LBUTTONDBLCLK:
		//		bDoubleClick = true;
		//		MouseButton = EMouseButtons::Left;
		//		break;
		//	case WM_LBUTTONUP:
		//		bMouseUp = true;
		//		MouseButton = EMouseButtons::Left;
		//		break;
		//	case WM_LBUTTONDOWN:
		//		MouseButton = EMouseButtons::Left;
		//		//Pos = MAKEPOINTS(lParam);
		//		break;
		//	}

		//	if (bMouseUp)
		//	{
		//		return MessageHandler->OnMouseUp(MouseButton, CursorPos);
		//	}
		//	else if (bDoubleClick)
		//	{
		//		MessageHandler->OnMouseDoubleClick(CurrentWindow, MouseButton, CursorPos);
		//	}
		//	else
		//	{
		//		MessageHandler->OnMouseDown(CurrentWindow, MouseButton, CursorPos);
		//	}
		//	

		//	return 0;
		//}

		//break;

		case WM_NCHITTEST:
		{
			POINT Point;
			static RECT border_thickness = { 4, 4, 4, 4 };
			Point.x = GET_X_LPARAM(lParam);
			Point.y = GET_Y_LPARAM(lParam);
			ScreenToClient(hWnd, &Point);

			if (!CurrentWindow->IsMaximized())
			{
				RECT Rect;
				GetClientRect(hWnd, &Rect);

				const int VerticalBorderSize = GetSystemMetrics(SM_CYFRAME);
				
				enum { left = 1, top = 2, right = 4, bottom = 8 };
				int hit = 0;
				if (Point.x <= border_thickness.left)
					hit |= left;
				if (Point.x >= Rect.right - border_thickness.right)
					hit |= right;
				if (Point.y <= border_thickness.top || Point.y < VerticalBorderSize)
					hit |= top;
				if (Point.y >= Rect.bottom - border_thickness.bottom)
					hit |= bottom;

				if (hit & top && hit & left)        return HTTOPLEFT;
				if (hit & top && hit & right)       return HTTOPRIGHT;
				if (hit & bottom && hit & left)     return HTBOTTOMLEFT;
				if (hit & bottom && hit & right)    return HTBOTTOMRIGHT;
				if (hit & left)                     return HTLEFT;
				if (hit & top)                      return HTTOP;
				if (hit & right)                    return HTRIGHT;
				if (hit & bottom)                   return HTBOTTOM;
			}
			ECursorArea TitleBarHittest;
			
			TitleBarHittest = CurrentWindow->bTitleBarHovarered;

			switch (TitleBarHittest)
			{
			case ECursorArea::Client:	return HTCLIENT;
			case ECursorArea::Close:	return HTCLOSE;
			case ECursorArea::Caption:	return HTCAPTION;
			case ECursorArea::Maximize:	return HTZOOM;
			case ECursorArea::Minimize:	return HTREDUCE;
			case ECursorArea::Menu:		return HTMENU;
			default:					return HTCLIENT;
			}
		}

		break;

		case WM_MOVE:
		{
			FIntPoint NewPosition = FIntPoint((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

			MessageHandler->OnMovedWindow(CurrentWindow, NewPosition);
		}
		break;

		case WM_MOVING:
		{
			RECT* Rect = (RECT*)lParam;

			MessageHandler->OnMovingWindow(CurrentWindow, FIntRect(Rect->left, Rect->top, Rect->right, Rect->bottom));
		}
		break;

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

				const bool bWasMinimized = (wParam == SIZE_MINIMIZED);
				MessageHandler->OnSizeChanged(CurrentWindow, NewWidth, NewHeight, bWasMinimized);
			}
			break;
		}
		case WM_SIZING:
		{
			WINDOWINFO WindowInfo = {};
			WindowInfo.cbSize = sizeof(WindowInfo);
			GetWindowInfo(hWnd, &WindowInfo);

			RECT TestRect;
			TestRect.left = TestRect.right = TestRect.top = TestRect.bottom = 0;
			AdjustWindowRectEx(&TestRect, WindowInfo.dwStyle, false, WindowInfo.dwExStyle);

			RECT* Rect = (RECT*)lParam;
			Rect->left -= TestRect.left;
			Rect->right -= TestRect.right;
			Rect->top -= TestRect.top;
			Rect->bottom -= TestRect.bottom;

			int32 NewWidth = Rect->right - Rect->left;
			int32 NewHeight = Rect->bottom - Rect->top;

			int32 MinWidth = 800;
			int32 MinHeight = 600;

			if (NewWidth < MinWidth)
			{
				if (wParam == WMSZ_LEFT || wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_TOPLEFT)
				{
					Rect->left -= (MinWidth - NewWidth);
				}
				else if (wParam == WMSZ_RIGHT || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_TOPRIGHT)
				{
					Rect->right += (MinWidth - NewWidth);
				}

				NewWidth = MinWidth;
			}

			if (NewHeight < MinHeight)
			{
				if (wParam == WMSZ_TOP || wParam == WMSZ_TOPLEFT || wParam == WMSZ_TOPRIGHT)
				{
					Rect->top -= (MinHeight - NewHeight);
				}
				else if(wParam == WMSZ_BOTTOM || wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_BOTTOMRIGHT)
				{
					Rect->bottom += (MinHeight - NewHeight);
				}

				NewHeight = MinHeight;
			}

			AdjustWindowRectEx(Rect, WindowInfo.dwStyle, false, WindowInfo.dwExStyle);

			return TRUE;
		}

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
	




	return (int32)DefWindowProc(hWnd, Msg, wParam, lParam);
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


FWindowsApplication::FWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle, const HCURSOR HCursor) :
	InstanceHandle(HInstance)
{
	bool bClassRegister = RegisterClass(InstanceHandle, IconHandle, HCursor);


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


bool FWindowsApplication::RegisterClass(const HINSTANCE HInstance, const HICON HIcon, const HCURSOR HCursor)
{
	WNDCLASS WC;
	FMemory::Memzero(&WC, sizeof(WC));
	WC.style = CS_DBLCLKS;
	WC.lpfnWndProc = AppWndProc;
	WC.cbClsExtra = 0;
	WC.cbWndExtra = 0;
	WC.hInstance = HInstance;
	WC.hIcon = HIcon;
	WC.hCursor = HCursor;
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