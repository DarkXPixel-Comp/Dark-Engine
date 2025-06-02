#include "Windows/WindowsWindow.h"
#include "Platform/PlatformApplicationMisc.h"
#include <dwmapi.h>
#include "Math/Vector2.h"



const TCHAR FWindowsWindow::AppWndClass[] = TEXT("DarkWindow");

FWindowsWindow::FWindowsWindow() : 
	hWnd(NULL),
	bIsVisible(false),
	bIsFirstTimeVisible(true),
	bInitiallyMaximized(false),
	bInitiallyMinimized(false),
	DPIScale(1.0f),
	bIsInitialized(false)
{




}

FWindowsWindow::~FWindowsWindow()
{
}

void FWindowsWindow::Destroy()
{
	DestroyWindow(hWnd);
}

bool FWindowsWindow::IsMaximize() const
{
	return IsZoomed(hWnd) != 0;
}

bool FWindowsWindow::IsMinimize() const
{
	return IsIconic(hWnd) != 0;
}

void FWindowsWindow::Initialize(FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition, HINSTANCE InHInstance, const TSharedPtr<FWindowsWindow>& InParent, const bool bShow)
{
	WndDefinition = InDefinition;
	OwningApplication = Application;

	uint32 WindowExStyle = 0;
	uint32 WindowStyle = 0;


	RegionWidth = RegionHeight = -1;

	const float XInitialRect = InDefinition.XDesiredPositionOnScreen;
	const float YInitialRect = InDefinition.YDisiredPositionOnScreen;

	const float WidthInitial = InDefinition.WidthDesiredOnScreen;
	const float HeightInitial = InDefinition.HeightDesiredOnScreen;

	
	DPIScale = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(XInitialRect, YInitialRect);

	int32 ClientX = static_cast<int32>(XInitialRect);
	int32 ClientY = static_cast<int32>(YInitialRect);

	int32 ClientWidth = static_cast<int32>(WidthInitial);
	int32 ClientHeight = static_cast<int32>(HeightInitial);

	int32 WindowX = ClientX;
	int32 WindowY = ClientY;
	int32 WindowWidth = ClientWidth;
	int32 WindowHeight = ClientHeight;

	if (!InDefinition.bHasOSWindowBorder)
	{
		WindowExStyle |= WS_EX_WINDOWEDGE;
		
		WindowStyle |= WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		if (InDefinition.bAppearsInTaskbar)
		{
			WindowExStyle |= WS_EX_APPWINDOW;
		}
		else
		{
			WindowExStyle |= WS_EX_TOOLWINDOW;
		}

		if (InDefinition.bIsTopmostWindow)
		{
			WindowExStyle |= WS_EX_TOPMOST;
		}

		if (!InDefinition.bAcceptsInput)
		{
			WindowExStyle |= WS_EX_TRANSPARENT;
		}
	}
	else
	{
		WindowExStyle |= WS_EX_APPWINDOW;
		WindowStyle |= WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION;

		if (InDefinition.bIsRegularWindow)
		{
			if(InDefinition.bSupportMaximize)
			{
				WindowStyle |= WS_MAXIMIZEBOX;
			}
			if (InDefinition.bSupportMinimize)
			{
				WindowStyle |= WS_MINIMIZEBOX;
			}
			if (InDefinition.bHasSizingFrame)
			{
				WindowStyle |= WS_THICKFRAME;
			}
			else
			{
				WindowStyle |= WS_BORDER;
			}
		}
		else
		{
			WindowStyle |= WS_POPUP | WS_BORDER;
		}

		RECT BorderRect = {};
		AdjustWindowRectEx(&BorderRect, WindowStyle, false, WindowExStyle);

		WindowX += BorderRect.left;
		WindowY += BorderRect.top;

		WindowWidth += BorderRect.right - BorderRect.left;
		WindowHeight += BorderRect.bottom - BorderRect.top;
	}

	hWnd = CreateWindowEx(WindowExStyle, AppWndClass, *InDefinition.Title, WindowStyle,
		WindowX, WindowY, WindowWidth, WindowHeight, InParent ? InParent->hWnd : NULL, NULL, InHInstance, NULL);

	if (hWnd == NULL)
	{
		const uint32 Error =  GetLastError();
		return;
	}

	VirtualWidth = ClientWidth;
	VirtualHeight = ClientHeight;

	ReshapeWindow(ClientX, ClientY, ClientWidth, ClientHeight);

	if (!WndDefinition.bHasOSWindowBorder)
	{
		const DWMNCRENDERINGPOLICY RenderingPolicy = DWMNCRP_DISABLED;
		DwmSetWindowAttribute(hWnd, DWMWA_NCRENDERING_POLICY, &RenderingPolicy, sizeof(RenderingPolicy));

		const BOOL bEnableNCPaint = false;
		DwmSetWindowAttribute(hWnd, DWMWA_ALLOW_NCPAINT, &bEnableNCPaint, sizeof(bEnableNCPaint));
	}

	
	if (WndDefinition.bIsRegularWindow && !WndDefinition.bHasOSWindowBorder)
	{
		WindowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		if (WndDefinition.bSupportMaximize)
		{
			WindowStyle |= WS_MAXIMIZEBOX;
		}
		if (WndDefinition.bSupportMinimize)
		{
			WindowStyle |= WS_MINIMIZEBOX;
		}
		if (WndDefinition.bHasSizingFrame)
		{
			WindowStyle |= WS_THICKFRAME;
		}

		SetWindowLong(hWnd, GWL_STYLE, WindowStyle);
		uint32 SetWindowPositionFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;

		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SetWindowPositionFlags);
		DeleteMenu(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND);

		AdjustWindowRegion(ClientWidth, ClientHeight);
	}
	else if (WndDefinition.bHasOSWindowBorder)
	{
		if (!WndDefinition.bHasCloseButton)
		{
			EnableMenuItem(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_GRAYED);
		}
	}

	if (WndDefinition.bIsRegularWindow)
	{
		AddClipboardFormatListener(hWnd);
	}
	bIsInitialized = true;
}

void FWindowsWindow::ReshapeWindow(int32 NewX, int32 NewY, int32 NewWidth, int32 NewHeight)
{
	WINDOWINFO WindowInfo = {};

	WindowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &WindowInfo);

	AspectRatio = (float)NewWidth / (float)NewHeight;

	if (WndDefinition.bHasOSWindowBorder)
	{
		RECT BorderRect = {};
		AdjustWindowRectEx(&BorderRect, WindowInfo.dwStyle, false, WindowInfo.dwExStyle);
		NewX += BorderRect.left;
		NewY += BorderRect.top;
		NewWidth += BorderRect.right - BorderRect.left;
		NewHeight += BorderRect.bottom - BorderRect.top;
	}


	int32 WindowX = NewX;
	int32 WindowY = NewY;

	const bool bVirtualSizeChanged = (VirtualWidth != NewWidth || VirtualHeight != NewHeight);
	VirtualWidth = NewWidth;
	VirtualHeight = NewHeight;

	if (WndDefinition.bSizeWillChangeOften)
	{
		const RECT OldWindowRect = WindowInfo.rcWindow;
		const int32 OldWidth = OldWindowRect.right - OldWindowRect.left;
		const int32 OldHeight = OldWindowRect.bottom - OldWindowRect.top;

		const int32 MinRetainedWidth = WndDefinition.ExpectedMaxWidth != -1 ? WndDefinition.ExpectedMaxWidth : OldWidth;
		const int32 MinRetainedHeight = WndDefinition.ExpectedMaxHeight != -1 ? WndDefinition.ExpectedMaxHeight : OldHeight;

		NewWidth = max(NewWidth, MinRetainedWidth);
		NewHeight = max(NewHeight, MinRetainedHeight);
	}
	if (IsMaximize())
	{
		Restore();
	}

	SetWindowPos(hWnd, NULL, WindowX, WindowY, NewWidth, NewHeight, SWP_NOZORDER | SWP_NOACTIVATE | ((Mode == EWindowMode::FullScreen) ? SWP_NOSENDCHANGING : 0));
	
	bool bAdjustSizeChange = WndDefinition.bSizeWillChangeOften || bVirtualSizeChanged;
	bool bAdjustCoreners = WndDefinition.Type != EWindowType::Menu && WndDefinition.CornerRadius > 0;

	if (!WndDefinition.bHasOSWindowBorder && (bAdjustCoreners || bAdjustCoreners))
	{
		AdjustWindowRegion(VirtualWidth, VirtualHeight);
	}
}

HWND FWindowsWindow::GetHWND() const
{
	return hWnd;
}

FGenericWindowDefinition FWindowsWindow::GetWndDefinition() const
{
	return WndDefinition;
}

FWindowsLayout* FWindowsWindow::GetWndLayout() 
{
	return &WndLayout;
}

TSharedPtr<FWindowsWindow> FWindowsWindow::Make()
{
	return TSharedPtr<FWindowsWindow>(new FWindowsWindow());
}

void FWindowsWindow::Restore()
{
	if(!bIsFirstTimeVisible)
	{
		ShowWindow(hWnd, SW_RESTORE);
	}
	else
	{
		bInitiallyMaximized = false;
		bInitiallyMinimized = false;
	}


}

void FWindowsWindow::AdjustWindowRegion(int32 Width, int32 Height)
{
	RegionWidth = Width;
	RegionHeight = Height;

	HRGN Region = MakeWindowRegionObject(true);

	SetWindowRgn(hWnd, Region, false);
}

HRGN FWindowsWindow::MakeWindowRegionObject(bool bIncludeBorderWhenMaximized) const
{
	HRGN Region = {};
	if (RegionWidth != -1 && RegionHeight != -1)
	{
		const bool bIsBorderlessGameWindow = WndDefinition.Type == EWindowType::GameWindow && !WndDefinition.bHasOSWindowBorder;
		if (IsMaximize())
		{
			if (bIsBorderlessGameWindow)
			{
				WINDOWINFO WindowInfo = {};
				WindowInfo.cbSize = sizeof(WINDOWINFO);
				GetWindowInfo(hWnd, &WindowInfo);

				const int32 WindowBorderSize = bIncludeBorderWhenMaximized ? WindowInfo.cxWindowBorders : 0;
				Region = CreateRectRgn(WindowBorderSize, WindowBorderSize, RegionWidth + WindowBorderSize, RegionHeight + WindowBorderSize);
			}
			else
			{
				const int32 WindowBorderSize = bIncludeBorderWhenMaximized ? GetWindowBorderSize() : 0;
				Region = CreateRectRgn(WindowBorderSize, WindowBorderSize, RegionWidth - WindowBorderSize, RegionHeight - WindowBorderSize);
			}
		}
		else
		{
			const bool bUseCornerRadius = WndDefinition.Mode == EWindowMode::Windowed && !bIsBorderlessGameWindow && WndDefinition.CornerRadius > 0;
			if (bUseCornerRadius)
			{
				Region = CreateRoundRectRgn(0, 0, RegionWidth + 1, RegionHeight + 1, WndDefinition.CornerRadius, WndDefinition.CornerRadius);
			}
			else
			{
				Region = CreateRectRgn(0, 0, RegionWidth, RegionHeight);
			}
		}
	}
	else
	{
		RECT Rect = {};
		GetWindowRect(hWnd, &Rect);
		Region = CreateRectRgn(0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top);
	}

	return Region;
}

APPLICATIONCORE_API int32 FWindowsWindow::GetWindowBorderSize() const
{
	if (WndDefinition.Type == EWindowType::GameWindow && !WndDefinition.bHasOSWindowBorder)
	{
		return 0;
	}

	WINDOWINFO WindowInfo = {};
	WindowInfo.cbSize = sizeof(WindowInfo);
	::GetWindowInfo(hWnd, &WindowInfo);

	return WindowInfo.cxWindowBorders;
}

APPLICATIONCORE_API void FWindowsWindow::AdjustSize(FVector2d& Size) const
{
	if (bIsInitialized && WndDefinition.bSizeWillChangeOften)
	{
		Size = FVector2d(VirtualWidth, VirtualHeight);
	}
	else if (hWnd)
	{
		RECT ClientRect;
		GetClientRect(hWnd, &ClientRect);
		Size.X = ClientRect.right - ClientRect.left;
		Size.Y = ClientRect.bottom - ClientRect.top;
	}
}

EWindowMode FWindowsWindow::GetWindowMode() const
{
	return Mode;
}

float FWindowsWindow::GetDPIScale() const
{
	return DPIScale;
}

void FWindowsWindow::SetDPIScale(float Value)
{
	DPIScale = Value;
}

bool FWindowsWindow::IsVisible() const
{
	return bIsVisible;
}

void FWindowsWindow::Show()
{
	if (!bIsVisible)
	{
		bIsVisible = true;

		int32 ShowWindowCommand = SW_SHOW;
		if (bIsFirstTimeVisible)
		{
			bIsFirstTimeVisible = false;

			if (bInitiallyMaximized)
			{
				ShowWindowCommand = SW_SHOWMAXIMIZED;
			}
			else if (bInitiallyMinimized)
			{
				ShowWindowCommand = SW_MINIMIZE;
			}
		}

		ShowWindow(hWnd, ShowWindowCommand);
	}
}


void FWindowsWindow::Hide()
{
	if (bIsVisible)
	{
		bIsVisible = false;
		ShowWindow(hWnd, SW_HIDE);
	}
}

void FWindowsWindow::Minimize()
{
	if (!bIsFirstTimeVisible)
	{
		ShowWindow(hWnd, SW_MINIMIZE);
	}
	else
	{
		bInitiallyMinimized = true;
		bInitiallyMaximized = false;
	}
}

void FWindowsWindow::Maximize()
{
	if (!bIsFirstTimeVisible)
	{
		ShowWindow(hWnd, SW_MAXIMIZE);
	}
	else
	{
		bInitiallyMinimized = false;
		bInitiallyMaximized = true;
	}
}

bool FWindowsWindow::IsPointInWindow(int32 X, int32 Y) const
{
	HRGN Region = MakeWindowRegionObject(false);
	bool Result = PtInRegion(Region, X, Y);
	DeleteObject(Region);
	return Result;
}

void FWindowsWindow::OnParentWindowMinimized()
{
	GetWindowPlacement(hWnd, &PreParentMinimizedWindowPlacement);
}

void FWindowsWindow::OnParentWindowRestored()
{
	SetWindowPlacement(hWnd, &PreParentMinimizedWindowPlacement);
}
