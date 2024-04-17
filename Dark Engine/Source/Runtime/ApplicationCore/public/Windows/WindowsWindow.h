#pragma once
#include <Core.h>
#include <Delegate/Delegate.h>
#include <Windows.h>
#include <Containers/DarkString.h>
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Windows/WindowsApplication.h"
#include "Memory/TUniquePtr.h"
#include "Math/MathFwd.h"

#undef CreateWindow

DECLARE_MULTICAST_DELEGATE_TwoParams(ResizeWindow, long, long);
DECLARE_MULTICAST_DELEGATE(FDestroyWindow);
DECLARE_DELEGATE(FUpdateWindow);


class FWindowsWindowManager;

using namespace std;

struct FGenericWindowDefinition;









class DENGINE_API FWindowsWindow  : public FGenericWindow, public IDropTarget
{
public:
	static const TCHAR AppWindowClass[];

public:
	FWindowsWindow() {}
	~FWindowsWindow();
	void Destroy();
	void Update();

	void Tick(float DeltaTime);

	virtual FIntPoint GetRelativeMousePos() const;
						 
	void Initialize(class FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition, HINSTANCE InHInstance);
	void BeginImGui();
public:
	virtual void* GetOSWindowHandle() const override { return HWnd; }
	void AdjustWindowRegion(int32 Width, int32 Height);
	HRGN MakeWindowRegionObject(bool bIncludeBorderWhenMaximized) const;
	void ReshapeWindow(int32 NewX, int32 NewY, int32 NewWidth, int32 NewHeight);
	virtual void Restore() { ShowWindow(HWnd, SW_RESTORE); }

	float GetAspectRatio() const { return  AspectRatio; }

#ifdef IMGUI
	virtual void InitImGui();
#endif


	virtual void Show() { ShowWindow(HWnd, SW_SHOW); }

	void Hide() { ShowWindow(HWnd, SW_HIDE); }

	void Maximize() override { ShowWindow(HWnd, SW_MAXIMIZE); }

	void Minimize() { ShowWindow(HWnd, SW_MINIMIZE); }

	HWND GetHWnd() { return HWnd; }

	void SetWindowFocus();

	static TSharedPtr<FGenericWindow> Make();

	bool IsPointInWindow(int32 X, int32 Y);

	int32 GetWindowBorderSize() const;
	int32 GetWindowTitleBarSize() const;

	virtual bool IsMaximized() const { return IsZoomed(HWnd); }


	UINT GetWitdh() { return VirtualWidth; }
	UINT GetHeight() { return VirtualHeight; }

	virtual void SetTitle(const TCHAR* const Title)
	{
		SetWindowText(HWnd, Title);
	}

	virtual void SetTitle(FString Title)
	{
		SetTitle(*Title);
	}


	UINT GetRefreshRate() { return 75; }

	ResizeWindow onResizeWindow;
	FDestroyWindow onDestroyWindow;
	FUpdateWindow onUpdateWindow;


	const FGenericWindowDefinition& GetDefinition() const { return WndDefinition; }


private:
	TSharedPtr<FGenericWindowDefinition> Definition;
	//std::shared_ptr<FGenericApplication> Definition;
	class FWindowsApplication* OwningApplication = nullptr;



public:
	virtual HRESULT DragEnter(IDataObject* DataObject, DWORD KewState, POINTL CursorPosition, DWORD* CursorEffect) override;
	virtual HRESULT DragOver(DWORD KeyState, POINTL CursorPosition, DWORD* CursorEffect) override;
	virtual HRESULT DragLeave() override;
	virtual HRESULT Drop(IDataObject* DataObject, DWORD KeyState, POINTL CursorPosition, DWORD* CursorEffect) override;


	HRESULT QueryInterface(REFIID iid, void** ppvObject) override;
	ULONG AddRef() override;
	ULONG Release() override;


private:
	HWND HWnd{NULL};
	float AspectRatio = 16 / 9;
	bool bIsVisible = false;
	int32 VirtualWidth = 0;
	int32 VirtualHeight = 0;
	int32 RegionWidth = 0;
	int32 RegionHeight = 0;
	bool bIsDestroyed = false;

	FGenericWindowDefinition WndDefinition;



};


