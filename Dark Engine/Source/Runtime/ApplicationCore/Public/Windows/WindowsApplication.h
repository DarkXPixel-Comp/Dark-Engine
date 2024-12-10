#pragma once

#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsIncludes.h"


class FWindowsApplication : public FGenericApplication
{
public:
	static FWindowsApplication* CreateWindowsApplication(const HINSTANCE hInstance, const HICON hIcon, const HCURSOR hCursor = NULL);

public:





};