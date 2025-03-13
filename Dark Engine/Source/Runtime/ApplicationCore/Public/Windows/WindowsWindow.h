#pragma once
#include "Platform/Platform.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Windows/WindowsIncludes.h"
#include "Windows/WindowsLayout.h"


class FWindowsWindow : public FGenericWindow
{
public:
	static const TCHAR AppWindowClass[];

public:
	APPLICATIONCORE_API FWindowsWindow() {}
	APPLICATIONCORE_API virtual ~FWindowsWindow() {}
	void Destroy() {}
	void Update() {}

	bool IsMaximize() const { return true; }

	void Tick(float DeltaTime) {}

	void Initialize(class FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition);

	HWND GetHWND() const;

	bool HasBorder() const {}

	FGenericWindowDefinition GetWndDefinition() const;

	FWindowsLayout* GetWndLayout();


private:
	HWND hWnd = NULL;

	FGenericWindowDefinition WndDefinition;

	FWindowsLayout WndLayout;
};