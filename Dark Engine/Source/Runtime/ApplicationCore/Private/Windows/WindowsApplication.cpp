#include "Windows/WindowsApplication.h"
//#include "Windows/Win"

TUniquePtr<FWindowsApplication> GWindowsApplication;

//
//static TSharedPtr< FWindowsWindow > FindWindowByHWND(const TArray< TSharedPtr< FWindowsWindow > >& WindowsToSearch, HWND HandleToFind)
//{
//	for (int32 WindowIndex = 0; WindowIndex < WindowsToSearch.Num(); ++WindowIndex)
//	{
//		TSharedPtr< FWindowsWindow > Window = WindowsToSearch[WindowIndex];
//		if (Window->GetHWnd() == HandleToFind)
//		{
//			return Window;
//		}
//	}
//
//	return TSharedPtr< FWindowsWindow >(nullptr);
//}


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

