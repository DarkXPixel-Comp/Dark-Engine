#pragma once
#include <Core.h>
#include <Delegate/Delegate.h>
#include <Windows.h>
#include <Containers/DarkString.h>
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Windows/WindowsApplication.h"
#include "Memory/TUniquePtr.h"

#undef CreateWindow

DECLARE_MULTICAST_DELEGATE_TwoParams(ResizeWindow, long, long);
DECLARE_MULTICAST_DELEGATE(FDestroyWindow);
DECLARE_DELEGATE(FUpdateWindow);


class FWindowsWindowManager;

using namespace std;

struct FGenericWindowDefinition;









class DENGINE_API FWindowsWindow  : public FGenericWindow, public IDropTarget
{
	friend FWindowsWindowManager;

public:
	static const TCHAR AppWindowClass[];

public:
	FWindowsWindow(FWindowsWindowManager* manager, UINT index);
	FWindowsWindow() {}
	~FWindowsWindow();
	void Create(UINT w, UINT h, UINT x, UINT y, string name, FWindowsWindow* Parent);
	void Destroy();
	void Update();

	void Tick(float DeltaTime);


	void Initialize(class FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition, HINSTANCE InHInstance);
	void BeginImGui();
public:
	virtual void* GetOSWindowHandle() const override { return HWnd; }
	void AdjustWindowRegion(int32 Width, int32 Height);
	HRGN MakeWindowRegionObject(bool bIncludeBorderWhenMaximized) const;
	void ReshapeWindow(int32 NewX, int32 NewY, int32 NewWidth, int32 NewHeight);
	void Restore() { ShowWindow(HWnd, SW_RESTORE); }

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

	void SetResolution(UINT w, UINT h);

	void SetPos(UINT x, UINT y);

	void SetResPos(UINT w, UINT h, UINT x, UINT y);

	bool isClose();
	bool IsMaximized() const { return IsZoomed(HWnd); }


	UINT GetWitdh() { return width; }
	UINT GetHeight() { return height; }

	UINT GetIndex() { return m_Index; }

	HWND GetHandle() { return m_Wnd; }

	void SetWindowTitle(FString str);

	virtual void SetTitle(const TCHAR* const Title);

	UINT GetRefreshRate() { return 75; }

	int MouseX = 0, MouseY = 0;


	ResizeWindow onResizeWindow;
	FDestroyWindow onDestroyWindow;
	FUpdateWindow onUpdateWindow;


	const FGenericWindowDefinition& GetDefinition() const { return WndDefinition; }


private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

 
	TSharedPtr<FGenericWindowDefinition> Definition;
	//std::shared_ptr<FGenericApplication> Definition;
	class FWindowsApplication* OwningApplication;



public:
	virtual HRESULT DragEnter(IDataObject* DataObject, DWORD KewState, POINTL CursorPosition, DWORD* CursorEffect) override;
	virtual HRESULT DragOver(DWORD KeyState, POINTL CursorPosition, DWORD* CursorEffect) override;
	virtual HRESULT DragLeave() override;
	virtual HRESULT Drop(IDataObject* DataObject, DWORD KeyState, POINTL CursorPosition, DWORD* CursorEffect) override;


	HRESULT QueryInterface(REFIID iid, void** ppvObject) override;
	ULONG AddRef() override;
	ULONG Release() override;



private:
	UINT width{ 0 };
	UINT height{ 0 };
	UINT leftX{ 0 };
	UINT topY{ 0 };


	HWND	m_Wnd;
	MSG		msg;


	string m_Name;

	FWindowsWindowManager* m_Manager;

	bool m_Close = true;

	UINT m_Index;





///////////////////



private:
	HWND HWnd;
	float AspectRatio = 16 / 9;
	bool bIsVisible;
	int32 VirtualWidth;
	int32 VirtualHeight;
	int32 RegionWidth;
	int32 RegionHeight;
	bool bIsDestroyed = false;

	FGenericWindowDefinition WndDefinition;



};




class DENGINE_API FWindowsWindowManager
{
	HANDLE hTimer;
	bool timerIsStarted = false;
	LARGE_INTEGER li;
	UINT_PTR timerPtr;
	long delay;

public:
	FWindowsWindowManager();
	~FWindowsWindowManager();
	FWindowsWindow* CreateWindow(UINT Weight, UINT Height, FString Name);
	//FWindowsWindow* GetPrimalWindow() { return windows.size() != 0 ? windows[0] : nullptr; }
	FWindowsWindow* GetWindow(UINT index = 0) { return index > windows.GetSize() ? nullptr : windows[index]; }
	void Update();
	void Destroy(UINT index);
	void Quit();
	void SetDelay(int ms);
	bool WindowsIsClose()
	{
		for (auto i : windows)
		{
			if (!i->isClose())
				return false;
		}

		return true;
	}




private:
	TArray<FWindowsWindow*> windows;


	MSG msg;

};
