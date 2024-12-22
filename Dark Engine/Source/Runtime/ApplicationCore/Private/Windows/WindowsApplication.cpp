#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"
#include "CoreGlobals.h"

TUniquePtr<FWindowsApplication> GWindowsApplication;


static TSharedPtr< FWindowsWindow > FindWindowByHWND(const TArray< TSharedPtr< FWindowsWindow > >& WindowsToSearch, HWND HandleToFind)
{
	for (int32 WindowIndex = 0; WindowIndex < WindowsToSearch.Num(); ++WindowIndex)
	{
		TSharedPtr< FWindowsWindow > Window = WindowsToSearch[WindowIndex];
		if (Window->GetHWND() == HandleToFind)
		{
			return Window;
		}
	}

	return TSharedPtr<FWindowsWindow>(nullptr);
}


FWindowsApplication* FWindowsApplication::CreateWindowsApplication(const HINSTANCE hInstance, const HICON hIcon, const HCURSOR hCursor)
{
	if (!GWindowsApplication)
	{
		GWindowsApplication = MakeUnique(new FWindowsApplication(hInstance, hIcon, hCursor));
	}

	return GWindowsApplication.get();
}

LRESULT FWindowsApplication::AppWndProc(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	return GWindowsApplication->ProcessMessage(hInWnd, msg, wParam, lParam);
}

int32 FWindowsApplication::ProcessMessage(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	TSharedPtr<FWindowsWindow> CurrentWindow = FindWindowByHWND(Windows, hInWnd);

	if (CurrentWindow)
	{
		switch (msg)
		{
		case WM_DESTROY:
		{
			Windows.Remove(CurrentWindow);
			if (Windows.Num() <= 0)
			{
				RequestExit();
			}
			break;
		}
		case WM_QUIT:
		{
			RequestExit();
			break;
		}
		case WM_NCHITTEST:
		{
			if (CurrentWindow->GetWndDefinition().bSizable)
			{
				POINT Point;
				RECT BorderThickness = {
					CurrentWindow->GetWndDefinition().LeftBorder,
					CurrentWindow->GetWndDefinition().TopBorder,
					CurrentWindow->GetWndDefinition().RightBorder,
					CurrentWindow->GetWndDefinition().BottomBorder };

				Point.x = GET_X_LPARAM(lParam);
				Point.y = GET_Y_LPARAM(lParam);
				ScreenToClient(hInWnd, &Point);

				if (!CurrentWindow->IsMaximize())
				{
					RECT Rect;
					GetClientRect(hInWnd, &Rect);

					enum {left = 1, top = 2, right = 4, bottom = 8 };
					int32 hit = 0;
					
					if (Point.x <= BorderThickness.left)
						hit |= left;
					if (Point.x >= Rect.right - BorderThickness.right)
						hit |= right;
					if (Point.y <= BorderThickness.top)
						hit |= top;
					if (Point.y >= Rect.bottom - BorderThickness.bottom)
						hit |= bottom;

					if (hit & top && hit & left)
						return HTTOPLEFT;
					if (hit & top && hit & right)
						return HTTOPRIGHT;
					if (hit & bottom && hit & left)
						return HTBOTTOMLEFT;
					if (hit & bottom && hit & right)
						return HTBOTTOMRIGHT;
					if (hit & left)
						return HTLEFT;
					if (hit & top)
						return HTTOP;
					if (hit & right)
						return HTRIGHT;
					if (hit & bottom)
						return HTBOTTOM;
				}
				return HTCLIENT;
			}
			break;
		}
		case WM_MOVE:
		{

		}

		default:

			break;
		}





	}


	return int32();
}

