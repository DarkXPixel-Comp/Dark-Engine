#include "Windows/WindowsWindow.h"


void FWindowsWindow::Initialize(FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition)
{
	WndDefinition = InDefinition;



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
