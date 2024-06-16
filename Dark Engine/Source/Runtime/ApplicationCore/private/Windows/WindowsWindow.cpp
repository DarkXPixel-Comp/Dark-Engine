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


HRGN CreateWindowRgn(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	HRGN hrgn = CreateRectRgn(0, 0, -rc.right, -rc.bottom);
	if (hrgn == NULL)
		return NULL;

	HRGN hrgn2;
	GetWindowRgn(hwnd, hrgn2);
	CombineRgn(hrgn, hrgn, hrgn2, RGN_DIFF);
	return hrgn;
}



void FWindowsWindow::AdjustWindowRegion(int32 Width, int32 Height)
{
	RegionWidth = Width;
	RegionHeight = Height;

	HRGN Region = MakeWindowRegionObject(true);

	check(SetWindowRgn(HWnd, Region, false));
	//SetWindowRgn(HWnd, CreateWindowRgn(HWnd), TRUE);


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
void FWindowsWindow::ShutdownImGui()
{
	ImGui_ImplWin32_Shutdown();
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
		check(SUCCEEDED(DwmSetWindowAttribute(HWnd, DWMWA_NCRENDERING_POLICY, &RenderingPolicy, sizeof(RenderingPolicy))));

		const BOOL bEnableNCPaint = false;
		check(SUCCEEDED(DwmSetWindowAttribute(HWnd, DWMWA_ALLOW_NCPAINT, &bEnableNCPaint, sizeof(bEnableNCPaint))));


		WindowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

		check(SetWindowLong(HWnd, GWL_STYLE, WindowStyle));


		uint32 SetWindowPosFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;

		SetWindowPos(HWnd, nullptr, 0, 0, 0, 0, SetWindowPosFlags);

		RECT rcWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);

		if (WindowX < rcWorkArea.left) {
			WindowX = rcWorkArea.left;
		}
		if (WindowY < rcWorkArea.top) {
			WindowY = rcWorkArea.top;
		}
		if (WindowX + VirtualWidth > rcWorkArea.right) {
			WindowX = rcWorkArea.right - VirtualWidth;
		}
		if (WindowY + VirtualHeight > rcWorkArea.bottom) {
			WindowY = rcWorkArea.bottom - VirtualHeight;
		}
		SetWindowPos(HWnd, nullptr, WindowX, WindowY, VirtualWidth, VirtualHeight, SetWindowPosFlags);


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


FWindowsWindow::~FWindowsWindow()
{
	this->Destroy();
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



