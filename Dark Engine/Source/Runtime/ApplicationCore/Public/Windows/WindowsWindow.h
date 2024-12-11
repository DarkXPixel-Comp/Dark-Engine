#pragma once
#include "Platform/Platform.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Windows/WindowsIncludes.h"


class APPLICATIONCORE_API FWindowsWindow : public FGenericWindow
{
public:
	static const TCHAR AppWindowClass[];

public:
	FWindowsWindow() {}
	virtual ~FWindowsWindow();
	void Destroy();
	void Update();

	void Tick(float DeltaTime);

	void Initialize(class FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition);

private:
	HWND hWnd = NULL;

	FGenericWindowDefinition WndDefinition;
};