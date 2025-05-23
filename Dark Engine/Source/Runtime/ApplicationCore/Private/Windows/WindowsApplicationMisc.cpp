#pragma once

#include "Windows/WindowsApplicationMisc.h"
#include "Windows/WindowsApplication.h"
#include "Windows/WindowsGlobals.h"
#include "Runtime/Launch/Resources/Windows/resource.h" //<- TEMP
#include "Platform/PlatformProcess.h"

typedef HRESULT(STDAPICALLTYPE* GetDpiForMonitorProc)(HMONITOR Monitor, int32 DPIType, uint32* DPIX, uint32* DPIY);
APPLICATIONCORE_API GetDpiForMonitorProc GetDpiForMonitor = nullptr;
APPLICATIONCORE_API void* ShCoreDll = nullptr;


FGenericApplication* FWindowsApplicationMisc::CreateApplication()
{
	FGenericApplication* Result = nullptr;

	HICON AppIconHandle = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	Result = FWindowsApplication::CreateWindowsApplication(hInstance, AppIconHandle);

	return Result;
}

float FWindowsApplicationMisc::GetDPIScaleFactorAtPoint(float X, float Y)
{
	float Scale = 1.0f;

	if (GetDpiForMonitor)
	{
		POINT Position = { static_cast<LONG>(X), static_cast<LONG>(Y) };
		HMONITOR Monitor = MonitorFromPoint(Position, MONITOR_DEFAULTTONEAREST);
		if (Monitor)
		{
			uint32 DPIX = 0;
			uint32 DPIY = 0;
			if (SUCCEEDED(GetDpiForMonitor(Monitor, 0, &DPIX, &DPIY)))
			{
				Scale = static_cast<float>(DPIX) / USER_DEFAULT_SCREEN_DPI;
			}
	
		}
	}
	else
	{
		HDC Context = GetDC(nullptr);
		int32 DPI = GetDeviceCaps(Context, LOGPIXELSX);
		Scale = (float)DPI / USER_DEFAULT_SCREEN_DPI;
		ReleaseDC(nullptr, Context);
	}
	return Scale;
}

void FWindowsApplicationMisc::SetHighDPIMode()
{
	if ((ShCoreDll) || (ShCoreDll = FPlatformProcess::GetDllHandle(TEXT("shcore.dll"))))
	{
		typedef enum _PROCESS_DPI_AWARENESS {
			PROCESS_DPI_UNAWARE = 0,
			PROCESS_SYSTEM_DPI_AWARE = 1,
			PROCESS_PER_MONITOR_DPI_AWARE = 2
		} PROCESS_DPI_AWARENESS;
		typedef HRESULT(STDAPICALLTYPE* SetProcessDpiAwarenessProc)(PROCESS_DPI_AWARENESS Value);
		SetProcessDpiAwarenessProc SetProcessDpiAwareness = (SetProcessDpiAwarenessProc)FPlatformProcess::GetDllExport(ShCoreDll, TEXT("SetProcessDpiAwareness"));
		GetDpiForMonitor = (GetDpiForMonitorProc)FPlatformProcess::GetDllExport(ShCoreDll, TEXT("GetDpiForMonitor"));

		typedef HRESULT(STDAPICALLTYPE* GetProcessDpiAwarenessProc)(HANDLE hProcess, PROCESS_DPI_AWARENESS* Value);
		GetProcessDpiAwarenessProc GetProcessDpiAwareness = (GetProcessDpiAwarenessProc)FPlatformProcess::GetDllExport(ShCoreDll, TEXT("GetProcessDpiAwareness"));

		if (SetProcessDpiAwareness && GetProcessDpiAwareness)
		{
			PROCESS_DPI_AWARENESS CurrentAwareness = PROCESS_DPI_UNAWARE;
			GetProcessDpiAwareness(nullptr, &CurrentAwareness);

			if (CurrentAwareness != PROCESS_PER_MONITOR_DPI_AWARE)
			{
				HRESULT hr = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
			}

		}
	} 
	else if (void* User32Dll = FPlatformProcess::GetDllHandle(TEXT("user32.dll")))
	{
		typedef BOOL(WINAPI* SetProcessDpiAwareProc)(void);
		SetProcessDpiAwareProc SetProcessDpiAware = (SetProcessDpiAwareProc)FPlatformProcess::GetDllExport(User32Dll, TEXT("SetProcessDPIAware"));
		if (SetProcessDpiAware)
		{
			BOOL Result = SetProcessDPIAware();
		}
	}
}

int32 FWindowsApplicationMisc::GetMonitorDPI(const FMonitorInfo& MonitorInfo)
{
	int32 DisplayDPI = USER_DEFAULT_SCREEN_DPI;
	
	if (GetDpiForMonitor)
	{
		RECT MonitorDim;
		MonitorDim.left = MonitorInfo.DisplayRect.Left;
		MonitorDim.top = MonitorInfo.DisplayRect.Up;
		MonitorDim.right = MonitorInfo.DisplayRect.Right;
		MonitorDim.bottom = MonitorInfo.DisplayRect.Down;

		HMONITOR Monitor = MonitorFromRect(&MonitorDim, MONITOR_DEFAULTTONEAREST);
		if (Monitor)
		{
			uint32 DPIX = 0;
			uint32 DPIY = 0;
			if (SUCCEEDED(GetDpiForMonitor(Monitor, 0, &DPIX, &DPIY)))
			{
				DisplayDPI = DPIX;
			}
		}
	}
	else
	{
		HDC Context = GetDC(nullptr);
		int32 DPI = GetDeviceCaps(Context, LOGPIXELSX);
		DisplayDPI = DPI;
		ReleaseDC(nullptr, Context);
	}

	return DisplayDPI;
}

void FWindowsApplicationMisc::Shutdown()
{
	FPlatformProcess::FreeDllHandle(ShCoreDll);
}
