#pragma once
#include <Windows.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <memory>
#include <Core/Delegate/Delegate.h>


enum eInputActionState
{
	F1 = 0,
	F2,
	LShift
};

enum eInputAxisState
{

};


DECLARE_MULTICAST_DELEGATE_OneParam(FActionDelegate, eInputActionState);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAxisDelegate, eInputAxisState, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMouseDelegate, float, float);
DECLARE_MULTICAST_DELEGATE(FEscDelegate);
DECLARE_MULTICAST_DELEGATE(FWDelegate);
DECLARE_MULTICAST_DELEGATE(FSDelegate);
DECLARE_MULTICAST_DELEGATE(FDDelegate);
DECLARE_MULTICAST_DELEGATE(FADelegate);
DECLARE_MULTICAST_DELEGATE(FSpaceDelegate);



class FInputCore
{
	static FInputCore* _inst_;
public:
	FInputCore();

public:
	static LRESULT InputProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void SetWindow(HWND hWnd);
	void Update();
private:
	LRESULT InputCoreProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:
	FActionDelegate ActionDelegate;
	FAxisDelegate AxisDelegate;
	FMouseDelegate MouseDelegate;
	FEscDelegate EscDelegate;
	FWDelegate WDelegate;
	FSDelegate SDelegate;
	FADelegate ADelegate;
	FDDelegate DDelegate;
	FSpaceDelegate SpaceDelegate;

private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	HWND m_hWnd = NULL;
	bool m_isRelative = false;
};
