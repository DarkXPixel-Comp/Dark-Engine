#include "Windows/WindowsWindow.h"
#include "Platform/PlatformApplicationMisc.h"
#include <dwmapi.h>
#include "Math/Vector2.h"



const TCHAR FWindowsWindow::AppWndClass[] = TEXT("DarkWindow");

FWindowsWindow::FWindowsWindow() : 
	hWnd(NULL)
{




}

FWindowsWindow::~FWindowsWindow()
{
}

APPLICATIONCORE_API bool FWindowsWindow::IsMaximize() const
{
	return IsZoomed(hWnd) != 0;
}

void FWindowsWindow::Initialize(FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition, HINSTANCE InHInstance, const TSharedPtr<FWindowsWindow>& InParent, const bool bShow)
{
	WndDefinition = InDefinition;
	OwningApplication = Application;

	uint32 WindowExStyle = 0;
	uint32 WindowStyle = 0;


	RegionWidth = RegionHeight = -1;

	const float XInitialRect = InDefinition.PosX;
	const float YInitialRect = InDefinition.PosY;

	const float WidthInitial = InDefinition.Width;
	const float HeightInitial = InDefinition.Height;

	
	DPI = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(XInitialRect, YInitialRect);

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
		if (InDefinition.AppearsInTaskbar)
		{
			WindowExStyle |= WS_EX_APPWINDOW;
		}
		else
		{
			WindowExStyle |= WS_EX_TOOLWINDOW;
		}

		if (InDefinition.IsTopmostWindow)
		{
			WindowExStyle |= WS_EX_TOPMOST;
		}

		if (!InDefinition.AcceptsInput)
		{
			WindowExStyle |= WS_EX_TRANSPARENT;
		}
	}
	else
	{
		WindowExStyle |= WS_EX_APPWINDOW;
		WindowStyle |= WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION;

		if (InDefinition.IsRegularWindow)
		{
			if(InDefinition.SupportsMaximize)
			{
				WindowStyle |= WS_MAXIMIZEBOX;
			}
			if (InDefinition.SupportsMinimize)
			{
				WindowStyle |= WS_MINIMIZEBOX;
			}
			if (InDefinition.HasSizingFrame)
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

		WindowWidth = BorderRect.right - BorderRect.left;
		WindowHeight = BorderRect.bottom - BorderRect.top;
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

	if (WndDefinition.bHasOSWindowBorder)
	{
		const DWMNCRENDERINGPOLICY RenderingPolicy = DWMNCRP_DISABLED;
		DwmSetWindowAttribute(hWnd, DWMWA_NCRENDERING_POLICY, &RenderingPolicy, sizeof(RenderingPolicy));

		const BOOL bEnableNCPaint = false;
		DwmSetWindowAttribute(hWnd, DWMWA_ALLOW_NCPAINT, &bEnableNCPaint, sizeof(bEnableNCPaint));
	}

	
	if (WndDefinition.IsRegularWindow && !WndDefinition.bHasOSWindowBorder)
	{
		WindowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		if (WndDefinition.SupportsMaximize)
		{
			WindowStyle |= WS_MAXIMIZEBOX;
		}
		if (WndDefinition.SupportsMinimize)
		{
			WindowStyle |= WS_MINIMIZEBOX;
		}
		if (WndDefinition.HasSizingFrame)
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
		if (!WndDefinition.HasCloseButton)
		{
			EnableMenuItem(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_GRAYED);
		}
	}

	if (WndDefinition.IsRegularWindow)
	{
		AddClipboardFormatListener(hWnd);
	}
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
		NewWidth = BorderRect.right - BorderRect.left;
		NewHeight = BorderRect.bottom - BorderRect.top;
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

APPLICATIONCORE_API void FWindowsWindow::AdjustSize(FVector2f& Size) const
{
	if (WndDefinition.bSizeWillChangeOften)
	{
		Size = FVector2f(VirtualWidth, VirtualHeight);
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
