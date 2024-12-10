#pragma once

#include "Windows/WindowsApplicationMisc.h"
#include "Windows/WindowsApplication.h"
#include "Windows/WindowsGlobals.h"
#include "Runtime/Launch/Resources/Windows/resource.h" //<- TEMP


FGenericApplication* FWindowsApplicationMisc::CreateApplication()
{
	FGenericApplication* Result = nullptr;

	HICON AppIconHandle = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	Result = FWindowsApplication::CreateWindowsApplication(hInstance, AppIconHandle);

	return Result;
}
