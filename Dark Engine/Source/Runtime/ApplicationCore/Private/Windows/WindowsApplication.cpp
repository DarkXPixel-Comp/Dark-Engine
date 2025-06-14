#include "Windows/WindowsApplication.h"
#include "Windows/WindowsApplicationMisc.h"
#include "Windows/WindowsWindow.h"
#include "CoreGlobals.h"
#include <dwmapi.h>
#include <SetupAPI.h>
#include <cfgmgr32.h>

FWindowsApplication* GWindowsApplication;


const FIntPoint FWindowsApplication::MinimizedWindowPosition(-32000, -32000);

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





FWindowsApplication::~FWindowsApplication()
{
}

FWindowsApplication* FWindowsApplication::CreateWindowsApplication(const HINSTANCE hInstance, const HICON hIcon, const HCURSOR hCursor)
{
	if (!GWindowsApplication)
	{
		GWindowsApplication = new FWindowsApplication(hInstance, hIcon, hCursor);
	}

	return GWindowsApplication;
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

void FWindowsApplication::RebuildDisplayMetrics(FDisplayMetrics& OutDisplay)
{
	OutDisplay.PrimaryDisplayWidth = GetSystemMetrics(SM_CXSCREEN);
	OutDisplay.PrimaryDisplayHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT WorkAreaRect;
	if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkAreaRect, 0))
	{
		memset(&WorkAreaRect, 0, sizeof(WorkAreaRect));
	}
	OutDisplay.PrimaryDisplayWorkAreaRect.Left = WorkAreaRect.left;
	OutDisplay.PrimaryDisplayWorkAreaRect.Up = WorkAreaRect.top;
	OutDisplay.PrimaryDisplayWorkAreaRect.Right = WorkAreaRect.right;
	OutDisplay.PrimaryDisplayWorkAreaRect.Down = WorkAreaRect.bottom;

	OutDisplay.VirtualDisplayRect.Left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	OutDisplay.VirtualDisplayRect.Up = GetSystemMetrics(SM_YVIRTUALSCREEN);
	OutDisplay.VirtualDisplayRect.Right = OutDisplay.VirtualDisplayRect.Left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	OutDisplay.VirtualDisplayRect.Down = OutDisplay.VirtualDisplayRect.Up + GetSystemMetrics(SM_CYVIRTUALSCREEN);

	GetMonitorsInfo(OutDisplay.MonitorInfo);
}

void FWindowsApplication::PumpMessages(float DeltaTime)
{
	MSG Message;

	while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}

bool GetMonitorSizeFromEDID(const HKEY hevRegKey, int32& OutWidth, int32& OutHeight, int32& OutDPI)
{
	static TCHAR ValueName[512];

	DWORD Type;
	DWORD ActualValueNameLenght = 512;

	BYTE EDIDData[1024];
	DWORD EDIDSize = sizeof(EDIDData);

	for (LONG i = 0, RetValue = ERROR_SUCCESS; RetValue != ERROR_NO_MORE_ITEMS; ++i)
	{
		RetValue = RegEnumValue(hevRegKey, i, &ValueName[0], &ActualValueNameLenght, NULL, &Type, EDIDData, &EDIDSize);

		if (RetValue != ERROR_SUCCESS || FString(ValueName) != TEXT("EDID"))
		{
			continue;
		}

		// EDID data format documented here:
		// http://en.wikipedia.org/wiki/EDID

		int DetailTimingDescriptorStartIndex = 54;
		OutWidth = ((EDIDData[DetailTimingDescriptorStartIndex + 4] >> 4) << 8) | EDIDData[DetailTimingDescriptorStartIndex + 2];
		OutHeight = ((EDIDData[DetailTimingDescriptorStartIndex + 7] >> 4) << 8) | EDIDData[DetailTimingDescriptorStartIndex + 5];

		const int32 HorizontalSizeOffset = 21;
		const int32 VerticalSizeOffset = 22;
		const float CmToInch = 0.393701f;

		if (EDIDData[HorizontalSizeOffset] > 0 && EDIDData[VerticalSizeOffset] > 0)
		{
			float PhysicalWidth = CmToInch * (float)EDIDData[HorizontalSizeOffset];
			float PhysicalHeight = CmToInch * (float)EDIDData[VerticalSizeOffset];

			int32 HDpi = static_cast<int32>((float)OutWidth / PhysicalWidth);
			int32 VDpi = static_cast<int32>((float)OutHeight / PhysicalHeight);

			OutDPI = (HDpi + VDpi) / 2;
		}
		else
		{
			OutDPI = 0;
		}
		return true;
	}
	return false;
}

bool GetSizeForDevID(const FString& TargetDevID, int32 Width, int32& Height, int32& DPI)
{
	static const GUID ClassMonitorGUID = { 0x4d36e96e, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18} };

	HDEVINFO DevInfo = SetupDiGetClassDevsEx(
		&ClassMonitorGUID, NULL, NULL, DIGCF_PRESENT, NULL, NULL, NULL);

	if (DevInfo == NULL)
		return false;

	bool bRes = false;

	for (ULONG MonitorIndex = 0; ERROR_NO_MORE_ITEMS != GetLastError(); ++MonitorIndex)
	{
		SP_DEVINFO_DATA DevInfoData;
		ZeroMemory(&DevInfoData, sizeof(DevInfoData));
		DevInfoData.cbSize = sizeof(DevInfoData);

		if (SetupDiEnumDeviceInfo(DevInfo, MonitorIndex, &DevInfoData) == TRUE)
		{
			TCHAR Buffer[MAX_DEVICE_ID_LEN];
			if (CM_Get_Device_ID(DevInfoData.DevInst, Buffer, MAX_DEVICE_ID_LEN, 0) == CR_SUCCESS)
			{
				FString DevID(Buffer);
				DevID = DevID.Mid(8, DevID.Find(TEXT("\\"), 9) - 8);
				if (DevID == TargetDevID)
				{
					HKEY hDevRegKey = SetupDiOpenDevRegKey(DevInfo, &DevInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

					if (hDevRegKey && hDevRegKey != INVALID_HANDLE_VALUE)
					{
						bRes = GetMonitorSizeFromEDID(hDevRegKey, Width, Height, DPI);
						RegCloseKey(hDevRegKey);
						break;
					}
				}
			}
		}
	}

	if (SetupDiDestroyDeviceInfoList(DevInfo) == FALSE)
	{
		bRes = false;
	}

	return bRes;
}


void FWindowsApplication::GetMonitorsInfo(TArray<FMonitorInfo>& OutMonitorInfo)
{
	DISPLAY_DEVICE DisplayDevice = {};
	DisplayDevice.cb = sizeof(DISPLAY_DEVICE);

	DWORD DeviceIndex = 0;

	OutMonitorInfo.Empty();

	FString DeviceId;

	while (EnumDisplayDevices(0, DeviceIndex, &DisplayDevice, 0))
	{
		if ((DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
		{
			DISPLAY_DEVICE Monitor = {};
			Monitor.cb = sizeof(DISPLAY_DEVICE);
			DWORD MonitorIndex = 0;

			while (EnumDisplayDevices(DisplayDevice.DeviceName, MonitorIndex, &Monitor, 0))
			{
				if (Monitor.StateFlags & DISPLAY_DEVICE_ACTIVE && !(Monitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
				{
					FMonitorInfo MonitorInfo;
					MonitorInfo.Name = DisplayDevice.DeviceName;

					EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&MonitorInfo);

					MonitorInfo.ID = Monitor.DeviceID;
					MonitorInfo.Name = MonitorInfo.ID.Mid(8, MonitorInfo.ID.Find(TEXT("\\"), 9) - 8);

					if (GetSizeForDevID(MonitorInfo.Name, MonitorInfo.NativeWidth, MonitorInfo.NativeHeight, MonitorInfo.DPI))
					{
						MonitorInfo.ID = Monitor.DeviceID;
						MonitorInfo.bIsPrimary = (DisplayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) > 0;

						if (MonitorInfo.DPI < 96 || MonitorInfo.DPI > 300)
						{
							MonitorInfo.DPI = FWindowsApplicationMisc::GetMonitorDPI(MonitorInfo);
						}
						else
						{
							const float CenterX = 0.5f * float(MonitorInfo.DisplayRect.Right + MonitorInfo.DisplayRect.Left);
							const float CenterY = 0.5f * float(MonitorInfo.DisplayRect.Down + MonitorInfo.DisplayRect.Up);
							const float DPIScaleFactor = FWindowsApplicationMisc::GetDPIScaleFactorAtPoint(CenterX, CenterY);
							MonitorInfo.DPI = int32(float(MonitorInfo.DPI) * DPIScaleFactor);
						}


						OutMonitorInfo.Add(MonitorInfo);
					}
				}

				++MonitorIndex;
				memset(&Monitor, 0, sizeof(Monitor));
				Monitor.cb = sizeof(DISPLAY_DEVICE);
			}
		}
		++DeviceIndex;
		memset(&DisplayDevice, 0, sizeof(DisplayDevice));
		DisplayDevice.cb = sizeof(DISPLAY_DEVICE);
	}
}

BOOL FWindowsApplication::MonitorEnumProc(HMONITOR Monitor, HDC MonitorDC, LPRECT Rect, LPARAM UserData)
{
	MONITORINFOEX MonitorInfoEx = {};
	MonitorInfoEx.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(Monitor, &MonitorInfoEx);

	FMonitorInfo* Info = (FMonitorInfo*)UserData;
	if (Info && Info->Name == MonitorInfoEx.szDevice)
	{
		Info->DisplayRect.Down = MonitorInfoEx.rcMonitor.bottom;
		Info->DisplayRect.Up = MonitorInfoEx.rcMonitor.top;
		Info->DisplayRect.Left = MonitorInfoEx.rcMonitor.left;
		Info->DisplayRect.Right = MonitorInfoEx.rcMonitor.right;

		Info->WorkAreaRect.Down = MonitorInfoEx.rcWork.bottom;
		Info->WorkAreaRect.Up = MonitorInfoEx.rcWork.top;
		Info->WorkAreaRect.Left = MonitorInfoEx.rcWork.left;
		Info->WorkAreaRect.Right = MonitorInfoEx.rcWork.right;

		return FALSE;
	}
	return TRUE;
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

				if (CurrentWindow->GetWndDefinition().bIsRegularWindow)
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
			break;
		}
		case WM_MOUSEACTIVATE:
		{
			bForceActivateByMouse = !(LOWORD(lParam) & HTCLIENT);
			break;
		}
		case WM_ACTIVATE:
		{
			EWindowActivation ActivationType;

			if (LOWORD(wParam) && WA_ACTIVE)
			{
				ActivationType = bForceActivateByMouse ? EWindowActivation::ActivateByMouse : EWindowActivation::Activate;
			}
			else if (LOWORD(wParam) & WA_CLICKACTIVE)
			{
				ActivationType = EWindowActivation::ActivateByMouse;
			}
			else
			{
				ActivationType = EWindowActivation::Deactivate;
			}
			bForceActivateByMouse = false;
			UpdateAllModiefierKeyStates();

			BOOL Result = MessageHandler->OnWindowActivationChanged(CurrentWindow, ActivationType);
			return Result ? 0 : 1;
		}
		case WM_ACTIVATEAPP:
		{
			UpdateAllModiefierKeyStates();
			MessageHandler->OnApplicationActivationChanged(static_cast<bool>(wParam));
			break;
		}
		case WM_PAINT:
		{
			if (bInModalSizeLoop)
			{
				MessageHandler->OnOSPaint(CurrentWindow);
			}
			break;
		}
		case WM_ERASEBKGND:
			return 1;
		case WM_NCACTIVATE:
			if (!CurrentWindow->GetWndDefinition().bHasOSWindowBorder)
			{
				return true;
			}
			break;
		case WM_NCPAINT:
			if (!CurrentWindow->GetWndDefinition().bHasOSWindowBorder)
			{
				return false;
			}
			break;
		case WM_CLOSE:
			MessageHandler->OnWindowClose(CurrentWindow);
			return 0;

		case WM_SYSCOMMAND:
		{
			switch (wParam & 0xfff0)
			{	
			case SC_RESTORE:
				if (IsIconic(hInWnd))
				{
					ShowWindow(hInWnd, SW_RESTORE);
					return 0;
				}
				if (!MessageHandler->OnWindowAction(CurrentWindow, EWindowAction::Restore))
				{
					return 1;
				}
				break;
			case SC_MAXIMIZE:
				if (!MessageHandler->OnWindowAction(CurrentWindow, EWindowAction::Maximize))
				{
					return 1;
				}
				break;
			case SC_CLOSE:
				MessageHandler->OnWindowClose(CurrentWindow);
				return 1;
			default:
				break;
			}
			break;
		}
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* MinMaxInfo = (MINMAXINFO*)lParam;
			FWindowSizeLimits SizeLimits = MessageHandler->GetSizeLimitsForWindow(CurrentWindow);

			int32 BorderWidth = 0;
			int32 BorderHeight = 0;
			if (CurrentWindow->GetWndDefinition().bHasOSWindowBorder)
			{
				const DWORD WindowStyle = GetWindowLong(hInWnd, GWL_STYLE);
				const DWORD WindowExStyle = GetWindowLong(hInWnd, GWL_EXSTYLE);

				RECT BorderRect = { 0, 0, 0, 0 };
				::AdjustWindowRectEx(&BorderRect, WindowStyle, false, WindowExStyle);

				BorderWidth = BorderRect.right - BorderRect.left;
				BorderHeight = BorderRect.bottom - BorderRect.top;
			}
			const float DPIScaleFactor = CurrentWindow->GetDPIScale();

			MinMaxInfo->ptMinTrackSize.x = SizeLimits.GetMinWidth().value_or(MinMaxInfo->ptMinTrackSize.x) * DPIScaleFactor;
			MinMaxInfo->ptMinTrackSize.y = SizeLimits.GetMinHeight().value_or(MinMaxInfo->ptMinTrackSize.y) * DPIScaleFactor;
			MinMaxInfo->ptMaxTrackSize.x = SizeLimits.GetMaxWidth().value_or(MinMaxInfo->ptMaxTrackSize.x) * DPIScaleFactor;
			MinMaxInfo->ptMaxTrackSize.y = SizeLimits.GetMaxHeight().value_or(MinMaxInfo->ptMaxTrackSize.y) * DPIScaleFactor;
			return 0;
		}

		case WM_DISPLAYCHANGE:
		{
			FDisplayMetrics DisplayMetrics;
			FWindowsApplication::RebuildDisplayMetrics(DisplayMetrics);
			break;
		}

		case WM_CHAR:
		{
			const TCHAR CharCode = static_cast<TCHAR>(wParam);

			const bool bIsRepeat = (lParam & 0x40000000) != 0;

			MessageHandler->OnKeyChar(CharCode, bIsRepeat);
			return 0;
		}



		case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS;
		case WM_CREATE:
			return 0;

		case WM_MOVE:
		{
			const int32 NewX = static_cast<const int32>(LOWORD(lParam));
			const int32 NewY = static_cast<const int32>(HIWORD(lParam));

			FIntPoint NewPosition(NewX, NewY);
			if (FWindowsApplication::MinimizedWindowPosition != NewPosition)
			{
				if (MessageHandler->OnMovedWindow(CurrentWindow, NewPosition))
					return 0;
			}
			break;
		}

		case WM_SIZING:
		{
			MessageHandler->OnResizingWindow(CurrentWindow, {0, 0});
			if (CurrentWindow->GetWndDefinition().bShouldPreserveAspectRatio)
			{
				WINDOWINFO WindowInfo = {};
				WindowInfo.cbSize = sizeof(WINDOWINFO);
				GetWindowInfo(hInWnd, &WindowInfo);

				RECT* Rect = (RECT*)lParam;
				if (CurrentWindow->GetWndDefinition().bHasOSWindowBorder)
				{
					RECT NewRect = {};
					AdjustWindowRectEx(&NewRect, WindowInfo.dwStyle, false, WindowInfo.dwExStyle);

					Rect->left -= NewRect.left;
					Rect->top -= NewRect.top;
					Rect->right -= NewRect.right;
					Rect->bottom -= NewRect.bottom;
				}

				const float AspectRatio = CurrentWindow->GetAspectRation();
				int32 NewWidth = Rect->right - Rect->left;
				int32 NewHeight = Rect->bottom - Rect->top;

				FWindowSizeLimits SizeLimits = MessageHandler->GetSizeLimitsForWindow(CurrentWindow);

				switch (wParam)
				{
				case WMSZ_LEFT:
				case WMSZ_RIGHT:
				case WMSZ_BOTTOMLEFT:
				case WMSZ_BOTTOMRIGHT:
				case WMSZ_TOPLEFT:
				case WMSZ_TOPRIGHT:
				{
					int32 MinWidth = SizeLimits.GetMinWidth().value_or(0);
					if (SizeLimits.GetMinHeight().value_or(0) < SizeLimits.GetMinWidth().value_or(0))
					{
						MinWidth = SizeLimits.GetMinHeight().value_or(0) * AspectRatio;
					}
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
					break;
				}
				case WMSZ_TOP:
				case WMSZ_BOTTOM:
				{
					int32 MinHeight = SizeLimits.GetMinHeight().value_or(0);
					if (SizeLimits.GetMinWidth().value_or(0) < SizeLimits.GetMinHeight().value_or(0))
					{
						MinHeight = SizeLimits.GetMinWidth().value_or(0) / AspectRatio;
					}
					if (NewHeight < MinHeight)
					{
						if (wParam == WMSZ_TOP)
						{
							Rect->top -= (MinHeight - NewHeight);
						}
						else
						{
							Rect->bottom += (MinHeight - NewHeight);
						}

						NewHeight = MinHeight;
					}
					break;
				}
				}

				switch (wParam)
				{
				case WMSZ_LEFT:
				case WMSZ_RIGHT:
				{
					int32 AdjustedHeight = (int32)((float)NewWidth / AspectRatio);
					Rect->top -= (AdjustedHeight - NewHeight) / 2;
					Rect->bottom += (AdjustedHeight - NewHeight) / 2;
					break;
				}
				case WMSZ_TOP:
				case WMSZ_BOTTOM:
				{
					int32 AdjustedWidth = (int32)((float)NewHeight * AspectRatio);
					Rect->left -= (AdjustedWidth - NewWidth) / 2;
					Rect->right += (AdjustedWidth - NewWidth) / 2;
					break;
				}
				case WMSZ_TOPLEFT:
				{
					int32 AdjustedHeight = (int32)((float)NewWidth / AspectRatio);
					Rect->top -= AdjustedHeight - NewHeight;
					break;
				}
				case WMSZ_TOPRIGHT:
				{
					int32 AdjustedHeight = (int32)((float)NewWidth / AspectRatio);
					Rect->top -= AdjustedHeight - NewHeight;
					break;
				}
				case WMSZ_BOTTOMLEFT:
				{
					int32 AdjustedHeight = (int32)((float)NewWidth / AspectRatio);
					Rect->bottom += AdjustedHeight - NewHeight;
					break;
				}
				case WMSZ_BOTTOMRIGHT:
				{
					int32 AdjustedHeight = (int32)((float)NewWidth / AspectRatio);
					Rect->bottom += AdjustedHeight - NewHeight;
					break;
				}
				}

				if (CurrentWindow->GetWndDefinition().bHasOSWindowBorder)
				{
					AdjustWindowRectEx(Rect, WindowInfo.dwStyle, false, WindowInfo.dwExStyle);
				}
				return true;
			}

			break;
		}

		case WM_SHOWWINDOW:
		{
			switch (lParam)
			{
			case SW_PARENTCLOSING:
				CurrentWindow->OnParentWindowMinimized();
				break;
			case SW_PARENTOPENING:
				CurrentWindow->OnParentWindowRestored();
				break;
			}

			break;
		}

		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		{
			switch (wParam)
			{
			case HTMINBUTTON:
				if (!MessageHandler->OnWindowAction(CurrentWindow, EWindowAction::ClickedNonClientArea))
					return 1;
				break;
			case HTMAXBUTTON:
				if (!MessageHandler->OnWindowAction(CurrentWindow, EWindowAction::ClickedNonClientArea))
					return 1;
				break;
			case HTCLOSE:
				if (!MessageHandler->OnWindowAction(CurrentWindow, EWindowAction::ClickedNonClientArea))
					return 1;
				break;
			case HTCAPTION:
				if (!MessageHandler->OnWindowAction(CurrentWindow, EWindowAction::ClickedNonClientArea))
					return 1;
				break;
			default:
				break;
			}
			break;
		}

		default:

			break;
		}





	}


	return DefWindowProc(hInWnd, msg, wParam, lParam);
}

FWindowsApplication::FWindowsApplication(const HINSTANCE hInIntance, const HICON hInIcon, const HCURSOR hCursor) :
	HInstance(hInIntance),
	bForceActivateByMouse(false),
	bInModalSizeLoop(false)
{
	Register(hInIntance, hInIcon);

	RebuildDisplayMetrics(InitialDisplayMetrics);
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

void FWindowsApplication::DeferMessage(TSharedPtr<FWindowsWindow>& NativeWindow, HWND InHwnd, uint32 InMessage, WPARAM InWParam, LPARAM InLParam)
{
}

void FWindowsApplication::UpdateAllModiefierKeyStates()
{
	ModifierKeyState[EModifierKey::LeftShift] = (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0;
	ModifierKeyState[EModifierKey::RightShift] = (GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0;
	ModifierKeyState[EModifierKey::LeftControl] = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0;
	ModifierKeyState[EModifierKey::RightControl] = (GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0;
	ModifierKeyState[EModifierKey::LeftAlt] = (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0;
	ModifierKeyState[EModifierKey::RightAlt] = (GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;
	ModifierKeyState[EModifierKey::CapsLock] = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

