#pragma once
#include "Platform/Platform.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Windows/WindowsIncludes.h"
#include "Windows/WindowsLayout.h"


class APPLICATIONCORE_API FWindowsWindow : public FGenericWindow
{
public:
	static const TCHAR AppWindowClass[];

public:
	FWindowsWindow() {}
	virtual ~FWindowsWindow();
	void Destroy();
	void Update();

	bool IsMaximize() const;

	void Tick(float DeltaTime);

	void Initialize(class FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition);

	HWND GetHWND() const;

	bool HasBorder() const;

	FGenericWindowDefinition GetWndDefinition() const;

	FWindowsLayout* GetWndLayout();


private:
	HWND hWnd = NULL;

	FGenericWindowDefinition WndDefinition;

	FWindowsLayout WndLayout;
};