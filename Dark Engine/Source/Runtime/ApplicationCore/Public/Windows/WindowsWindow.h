#pragma once
#include "Platform/Platform.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Windows/WindowsIncludes.h"
#include "Windows/WindowsLayout.h"

#include <ole2.h>
#include <oleidl.h>

#undef IsMaximized


class FWindowsWindow : public FGenericWindow
//	, public IDropTarget
{
public:
	static const TCHAR AppWndClass[];

public:
	APPLICATIONCORE_API virtual ~FWindowsWindow();
	APPLICATIONCORE_API void Destroy() {}
	APPLICATIONCORE_API void Update() {}

	APPLICATIONCORE_API bool IsMaximize() const override;

	APPLICATIONCORE_API bool IsMinimize() const override;

	APPLICATIONCORE_API void Tick(float DeltaTime) {}

	APPLICATIONCORE_API void Initialize(class FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition, HINSTANCE InHInstance, const TSharedPtr<FWindowsWindow>& InParent, const bool bShow);

	APPLICATIONCORE_API void ReshapeWindow(int32 NewX, int32 NewY, int32 NewWidth, int32 NewHeight);

	APPLICATIONCORE_API HWND GetHWND() const;

	APPLICATIONCORE_API bool HasBorder() const { return false; }

	APPLICATIONCORE_API FGenericWindowDefinition GetWndDefinition() const;

	APPLICATIONCORE_API FWindowsLayout* GetWndLayout();

	APPLICATIONCORE_API static TSharedPtr<FWindowsWindow> Make();

	APPLICATIONCORE_API void Restore();

	APPLICATIONCORE_API void AdjustWindowRegion(int32 Width, int32 Height);

	APPLICATIONCORE_API HRGN MakeWindowRegionObject(bool bIncludeBorderWhenMaximized) const;

	APPLICATIONCORE_API int32 GetWindowBorderSize() const;

	APPLICATIONCORE_API void AdjustSize(FVector2d& Size) const override;

	virtual EWindowMode GetWindowMode() const override;

	APPLICATIONCORE_API virtual float GetDPIScale() const override;

	APPLICATIONCORE_API virtual void SetDPIScale(float Value) override;

	APPLICATIONCORE_API virtual bool IsVisible() const;

	APPLICATIONCORE_API virtual void Show();

	APPLICATIONCORE_API virtual void Hide();

	APPLICATIONCORE_API virtual bool IsPointInWindow(int32 X, int32 Y) const;

	APPLICATIONCORE_API virtual void OnParentWindowMinimized();

	APPLICATIONCORE_API virtual void OnParentWindowRestored();


protected:
	APPLICATIONCORE_API FWindowsWindow();

private:
	FWindowsApplication* OwningApplication;
	HWND hWnd = NULL;

	FGenericWindowDefinition WndDefinition;

	FWindowsLayout WndLayout;

	int32 RegionWidth;
	int32 RegionHeight;

	int32 VirtualWidth;
	int32 VirtualHeight;

	EWindowMode Mode;
	EWindowType Type;

	WINDOWPLACEMENT PreParentMinimizedWindowPlacement;

	//float DPI = 1.0f;

	float AspectRatio;


	bool bIsVisible : 1;
	bool bIsFirstTimeVisible : 1;
	bool bInitiallyMaximized : 1;
	bool bInitiallyMinimized : 1;


	float DPIScale;

	bool bIsInitialized;
};