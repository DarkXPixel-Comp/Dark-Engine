#include "InputCore.h"
#include <CommandLine/CommandLine.h>
#include <CoreDefines.h>


FInputCore* FInputCore::_inst_ = nullptr;

using namespace DirectX;

FInputCore::FInputCore()
{
	_inst_ = this;
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
}

void FInputCore::SetWindow(HWND hWnd)
{
	m_hWnd = hWnd;
	m_mouse->SetWindow(hWnd);
	m_mouse->SetMode(Mouse::MODE_ABSOLUTE);
}


void FInputCore::Update()
{
	auto KeyBoardState = m_keyboard->GetState();
	auto MouseState = m_mouse->GetState();




	if (KeyBoardState.IsKeyDown(Keyboard::F1))
	{
		m_mouse->SetMode(Mouse::MODE_RELATIVE);
		m_isRelative = true;
		ActionDelegate.BroadCast(eInputActionState::F1);
		MouseState.x = 0;
		MouseState.y = 0;
	}
	if (KeyBoardState.IsKeyDown(Keyboard::F2))
	{
		m_mouse->SetMode(Mouse::MODE_ABSOLUTE);
		m_isRelative = false;
		ActionDelegate.BroadCast(eInputActionState::F2);
	}
	if (KeyBoardState.Escape)
	{
		EscDelegate.BroadCast();
	}

	if (KeyBoardState.W) WDelegate.BroadCast();
	if (KeyBoardState.S) SDelegate.BroadCast();
	if (KeyBoardState.D) DDelegate.BroadCast();
	if (KeyBoardState.A) ADelegate.BroadCast();
	if (KeyBoardState.Space) SpaceDelegate.BroadCast();
	if (KeyBoardState.LeftShift) ActionDelegate.BroadCast(LShift);
	if (KeyBoardState.F3) ActionDelegate.BroadCast(F3);
	if (KeyBoardState.F4) ActionDelegate.BroadCast(F4); 
	if (KeyBoardState.IsKeyDown(Keyboard::F11) && f11) { ActionDelegate.BroadCast(F11); F11Delegate.BroadCast(); f11 = false; }
	if (KeyBoardState.IsKeyUp(Keyboard::F11)) { f11 = true;}

	
	if(m_isRelative)
		MouseDelegate.BroadCast(MouseState.x, MouseState.y);


}


LRESULT FInputCore::InputCoreProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(hWnd != m_hWnd)
		return 0;
	switch (msg)
	{
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(msg, wParam, lParam);
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(msg, wParam, lParam);
		break;
	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{

		}
		Keyboard::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(msg, wParam, lParam);
		break;
	}

	return 0;
}

LRESULT FInputCore::InputProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return _inst_->InputCoreProc(hWnd, msg, wParam, lParam);
}

