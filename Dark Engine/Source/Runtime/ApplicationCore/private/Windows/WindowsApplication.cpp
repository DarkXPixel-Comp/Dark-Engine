#include "Windows/WindowsApplication.h"
#include "CoreMinimal.h"

#include "Windows/WindowsWindow.h"


FWindowsApplication* WindowsApplication;


FWindowsApplication* FWindowsApplication::CreateWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle)
{
	WindowsApplication = new FWindowsApplication(HInstance, IconHandle);
	return nullptr;
}

void FWindowsApplication::DestroyWindowsApplication()
{
	delete WindowsApplication;
	WindowsApplication = nullptr;
}


FWindowsApplication::FWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle) :
	InstanceHandle(HInstance)
{
	bool bClassRegister = RegisterClass(InstanceHandle, IconHandle);

	// For Drag and Drop
	OleInitialize(NULL);





}

FWindowsApplication::~FWindowsApplication()
{
	OleUninitialize();


}


bool FWindowsApplication::RegisterClass(const HINSTANCE HInstance, const HICON HIcon)
{
	WNDCLASS WC;
	FMemory::Memzero(&WC, sizeof(WC));
	WC.style = CS_DBLCLKS;
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