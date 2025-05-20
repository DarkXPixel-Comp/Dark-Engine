#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"
#include "CoreGlobals.h"
#include <dwmapi.h>

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

TSharedPtr<FGenericWindow> FWindowsApplication::MakeWindow()
{
	return FWindowsWindow::Make();
}

void FWindowsApplication::InitializeWindow(const TSharedPtr<FGenericWindow>& InWindow, FGenericWindowDefinition InDefinition, const TSharedPtr<FGenericWindow>& InParent, bool bShow)
{
	const TSharedPtr<FWindowsWindow> Window = StaticCastSharedPtr<FWindowsWindow>(InWindow);
	const TSharedPtr<FWindowsWindow> ParentWindow = StaticCastSharedPtr<FWindowsWindow>(InParent);

	Windows.Add(Window);
	Window->Initialize(this, InDefinition, HInstance, ParentWindow, bShow);
}

void FWindowsApplication::AddMessageHandler(IWindowsMessageHandler& InMessageHandler)
{
	MessageHandlers.Add(&InMessageHandler);

}

void FWindowsApplication::RemoveMessageHandler(IWindowsMessageHandler& InMessageHandler)
{
	MessageHandlers.Remove(&InMessageHandler);
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

LRESULT FWindowsApplication::AppWndProc(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	return GWindowsApplication->ProcessMessage(hInWnd, msg, wParam, lParam);
}

int32 FWindowsApplication::ProcessMessage(HWND hInWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	TSharedPtr<FWindowsWindow> CurrentWindow = FindWindowByHWND(Windows, hInWnd);

	if (CurrentWindow)
	{
		for (auto& i : MessageHandlers)
		{
			i->ProcessMessage(hInWnd, msg, wParam, lParam);
		}

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
		case WM_NCCALCSIZE:
		{
			if (wParam && !CurrentWindow->GetWndDefinition().bHasOSWindowBorder)
			{
				if (CurrentWindow->GetWndDefinition().Type == EWindowType::GameWindow && CurrentWindow->IsMaximize())
				{
					WINDOWINFO WindowInfo = {};
					WindowInfo.cbSize = sizeof(WINDOWINFO);
					GetWindowInfo(hInWnd, &WindowInfo);

					LPNCCALCSIZE_PARAMS ResizingRects = (LPNCCALCSIZE_PARAMS)lParam;

					ResizingRects->rgrc[0].left += WindowInfo.cxWindowBorders;
					ResizingRects->rgrc[0].top += WindowInfo.cxWindowBorders;
					ResizingRects->rgrc[0].right -= WindowInfo.cxWindowBorders;
					ResizingRects->rgrc[0].bottom -= WindowInfo.cxWindowBorders;

					ResizingRects->rgrc[1].left = ResizingRects->rgrc[0].left;
					ResizingRects->rgrc[1].top = ResizingRects->rgrc[0].top;
					ResizingRects->rgrc[1].right = ResizingRects->rgrc[0].right;
					ResizingRects->rgrc[1].bottom = ResizingRects->rgrc[0].bottom;

					ResizingRects->lppos->x += WindowInfo.cxWindowBorders;
					ResizingRects->lppos->y += WindowInfo.cxWindowBorders;
					ResizingRects->lppos->cx -= 2 * WindowInfo.cxWindowBorders;
					ResizingRects->lppos->cy -= 2 * WindowInfo.cxWindowBorders;

					return WVR_VALIDRECTS;
				}
				else if (wParam)
				{
					LARGE_INTEGER Freq;
					QueryPerformanceFrequency(&Freq);

					// Ask DWM where the vertical blank falls
					DWM_TIMING_INFO TimingInfo;
					memset(&TimingInfo, 0, sizeof(TimingInfo));
					TimingInfo.cbSize = sizeof(TimingInfo);
					if (FAILED(DwmGetCompositionTimingInfo(NULL, &TimingInfo)))
					{
						return 0;
					}

					LARGE_INTEGER Now;
					QueryPerformanceCounter(&Now);

					// DWM told us about SOME vertical blank,
					// past or future, possibly many frames away.
					// Convert that into the NEXT vertical blank

					int64 Period = (int64)TimingInfo.qpcRefreshPeriod;

					int64 TimeToVSync = (int64)TimingInfo.qpcVBlank - (int64)Now.QuadPart;

					int64 FrameAdjustMultiplier;

					if (TimeToVSync >= 0)
					{
						FrameAdjustMultiplier = TimeToVSync / Period;
					}
					else
					{
						// Reach back to previous period
						// so WaitTime represents consistent position within phase
						FrameAdjustMultiplier = -1 + TimeToVSync / Period;
					}

					int64 WaitTime = TimeToVSync - (Period * FrameAdjustMultiplier);
					if (WaitTime == Period)
					{
						// this can happen when TimeToVSync is negative and a multiple of Period. 
						// It means that the Vsync is about to happen in the next 1 ms.
						// In this case, we wait 0 instead of waiting for the next Vsync after that one
						WaitTime = 0;
					}

					// Wait for the indicated time using a waitable timer as it 
					// is more accurate than a simple sleep.
					HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
					if (NULL != hTimer)
					{
						double WaitTimeMilliseconds = 0.0;
						if (Freq.QuadPart > 0)
						{
							WaitTimeMilliseconds = 1000.0 * (double)WaitTime / (double)Freq.QuadPart;
						}

						// Due time for WaitForSingleObject is in 100 nanosecond units.							
						double WaitTime100NanoSeconds = (1000.0f * 10.0f * WaitTimeMilliseconds);

						LARGE_INTEGER DueTime;

						// Negative value indicates relative time in the future.
						DueTime.QuadPart = (LONGLONG)(WaitTime100NanoSeconds) * -1;

						if (SetWaitableTimer(hTimer, &DueTime, 0, NULL, NULL, 0))
						{
							// Timeout time (second param) is in milliseconds. Set it to a bit longer than
							// the timer due time. If everything is working it won't be used.
							WaitForSingleObject(hTimer, (DWORD)(WaitTimeMilliseconds)+10);
						}

						CloseHandle(hTimer);
					}
				}

				return 0;
			}
			break;
		}




		case WM_NCHITTEST:
		{
			if (!CurrentWindow->GetWndDefinition().bHasOSWindowBorder)
			{
				RECT Rect;
				GetWindowRect(hInWnd, &Rect);

				const int32 LocalMouseX = (int32)(short)LOWORD(lParam) - Rect.left;
				const int32 LocalMouseY = (int32)(short)HIWORD(lParam) - Rect.top;

				if (CurrentWindow->GetWndDefinition().IsRegularWindow)
				{
					EWindowZone Zone = EWindowZone::ClientArea;

					if (MessageHandler->ShouldProcessUserInputMessages(CurrentWindow))
					{
						Zone = MessageHandler->GetWindowZoneForPoint(CurrentWindow, LocalMouseX, LocalMouseY);
					}

					static const LRESULT Results[] = { HTNOWHERE, HTTOPLEFT, HTTOP, HTTOPRIGHT, HTLEFT, HTCLIENT,
						  HTRIGHT, HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT,
						  HTCAPTION, HTMINBUTTON, HTMAXBUTTON, HTCLOSE, HTSYSMENU };


					return (int32)Results[(int32)Zone];

				}

			}
		}
		case WM_MOVE:
		{

		}

		default:

			break;
		}





	}


	return DefWindowProc(hInWnd, msg, wParam, lParam);
}

FWindowsApplication::FWindowsApplication(const HINSTANCE hInIntance, const HICON hInIcon, const HCURSOR hCursor) : HInstance(hInIntance)
{
	Register(hInIntance, hInIcon);
}

bool FWindowsApplication::Register(const HINSTANCE hInIntance, const HICON HIcon)
{
	WNDCLASS wc = {};
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = AppWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInIntance;
	wc.hIcon = HIcon;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = FWindowsWindow::AppWndClass;

	if (!RegisterClass(&wc))
	{
		return false;
	}

	return true;
}

