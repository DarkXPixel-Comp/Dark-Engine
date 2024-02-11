#include "Windows/WindowsApplication.h"
#include "CoreMinimal.h"

#include "Windows/WindowsWindow.h"


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
			return 0;
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