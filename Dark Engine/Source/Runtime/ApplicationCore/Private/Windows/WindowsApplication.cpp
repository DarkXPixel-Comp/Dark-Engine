#include "Windows/WindowsApplication.h"

TUniquePtr<FWindowsApplication> GWindowsApplication;

FWindowsApplication* FWindowsApplication::CreateWindowsApplication(const HINSTANCE hInstance, const HICON hIcon, const HCURSOR hCursor)
{
	if (!GWindowsApplication)
	{
		GWindowsApplication = MakeUnique(new FWindowsApplication());
	}

	return GWindowsApplication.get();
}

