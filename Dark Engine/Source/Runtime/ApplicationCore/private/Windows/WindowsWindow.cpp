#include "Windows/WindowsWindow.h"
#include <Application/Application.h>
#include <Logging/Logger.hpp>
#include <CoreDefines.h>
#include <DEngine.h>
#include <InputCore.h>
#include "imgui.h"
#include "imgui_impl_win32.h"


const TCHAR FWindowsWindow::AppWindowClass[] = TEXT("DarkWindow");


void FWindowsWindow::Initialize(FWindowsApplication* const Application, const FGenericWindowDefinition& InDefinition, HINSTANCE InHInstance)
{
//Definition = std::make_shared<FGenericWindowDefinition>(InDefinition);
	OwningApplication = Application;

	uint32 WindowStyle = 0;
	uint32 WindowExStyle = 0;

	const float XInitialRect = InDefinition.XPositionOnScreen;
	const float YInitialRect = InDefinition.YPositionOnScreen;

	const float WidthInitial = InDefinition.WidthOnScreen;
	const float HeightInitial = InDefinition.HeightOnScreen;

	int32 ClientX = FMath::TruncToInt(XInitialRect);
	int32 ClientY = FMath::TruncToInt(YInitialRect);
	int32 ClientWidth = FMath::TruncToInt(WidthInitial);
	int32 ClientHeight = FMath::TruncToInt(HeightInitial);

	int32 WindowX = ClientX;
	int32 WindowY = ClientY;
	int32 WindowWidth = ClientWidth;
	int32 WindowHeight = ClientHeight;

	if (InDefinition.bHasWindowBorder)
	{
		WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
		WindowExStyle = WS_EX_APPWINDOW;


	}
	else
	{
		WindowExStyle = WS_EX_WINDOWEDGE;

		WindowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		WindowExStyle |= WS_EX_APPWINDOW;

	}

	RECT BorderRect = { 0, 0, 0, 0 };
	
	AdjustWindowRectEx(&BorderRect, WindowStyle, false, WindowExStyle);

	ClientX += BorderRect.left;
	ClientY += BorderRect.top;
	WindowWidth += BorderRect.right - BorderRect.left;
	WindowHeight += BorderRect.bottom - BorderRect.top;

	const TCHAR* Title = TEXT("TEST"); // wstring(Definition->Title.begin(), Definition->Title.end()).data();


	HWnd = CreateWindowEx(WindowExStyle, AppWindowClass, Title, WindowStyle, WindowX,
		WindowY, WindowWidth, WindowHeight, NULL, NULL, InHInstance, NULL);

	if (HWnd == NULL)
	{
		const uint32 Error = GetLastError();


		return;
	}
}


TSharedPtr<FGenericWindow> FWindowsWindow::Make()
{
	return MakeShareble(new FWindowsWindow());
}




FWindowsWindow::FWindowsWindow(FWindowsWindowManager* maneger, UINT index)
{
	m_Manager = (maneger);
	m_Index = index;
}

void FWindowsWindow::Create(UINT w, UINT h, UINT x, UINT y, string name, FWindowsWindow* Parent)
{
	WNDCLASSEX wSex;

	wstring wClassName(name.begin(), name.end());

	LPCWSTR ClassName = wClassName.c_str();




	wSex.cbSize = sizeof(WNDCLASSEX);
	wSex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wSex.lpfnWndProc = FWindowsWindow::WndProc;
	wSex.cbClsExtra = 0;
	wSex.cbWndExtra = 0;
	wSex.hInstance = Application::GetInstance();
	wSex.hIcon = LoadIcon(Application::GetInstance(), MAKEINTRESOURCE(554));
	wSex.hCursor = LoadCursor(Application::GetInstance(), IDC_ARROW);
	wSex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wSex.lpszMenuName = NULL;
	wSex.lpszClassName = ClassName;
	wSex.hIconSm = LoadIcon(Application::GetInstance(), IDI_APPLICATION);




	if (!RegisterClassExW(&wSex))
	{
		auto er = GetLastError();

		Logger::log(L"Error register Class", LOGGER_ERROR);
		Logger::wait();
		abort();

	}



	m_Wnd = CreateWindowW(ClassName,
		ClassName,
		WS_OVERLAPPEDWINDOW,
		x, y, w, h,
		NULL,
		NULL,
		Application::GetInstance(),
		NULL);
	
	if (!m_Wnd)
	{
		Logger::log("Error create Window " + name);

		Logger::wait();

		abort();

		return;
	}


	SetWindowLongPtr(m_Wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLong(m_Wnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);


	ShowWindow(m_Wnd, Application::CmdShow());
	UpdateWindow(m_Wnd);


	RECT rect2;
	GetWindowRect(m_Wnd, &rect2);

	leftX = rect2.left;
	topY = rect2.top;
	width = rect2.right - rect2.left;
	height = rect2.bottom - rect2.top;

	m_Close = false;


	Log("Success create window ", cstr(name), " ", icstr(width), "x", icstr(height));
}

void FWindowsWindow::Destroy()
{
	if (m_Close)
		return;


	onDestroyWindow.BroadCast();

	m_Close = true;
	DestroyWindow(m_Wnd);


	Log("Window has been Destroyed. - ", cstr(m_Name));



}

void FWindowsWindow::Update()
{
	if (!IsWindow(m_Wnd))
	{
		m_Close = true;

		return;
	}

	RECT rect;

	GetWindowRect(m_Wnd, &rect);

	leftX = rect.left;
	topY = rect.top;


	if (!(width == rect.right - rect.left || height == rect.bottom - rect.top))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

	}





}

void FWindowsWindow::SetResolution(UINT w, UINT h)
{
	width = w;
	height = h;

	SetWindowPos(m_Wnd, NULL, leftX, topY, w, h, NULL);

}

void FWindowsWindow::SetPos(UINT x, UINT y)
{
	leftX = x;
	topY = y;

	SetWindowPos(m_Wnd, NULL, x, y, width, height, NULL);

}

void FWindowsWindow::SetResPos(UINT w, UINT h, UINT x, UINT y)
{
	leftX = x;
	topY = y;
	width = w;
	height = h;

	SetWindowPos(m_Wnd, NULL, x, y, w, h, NULL);
}

void FWindowsWindow::SetWindowTitle(FString str)
{
	SetWindowText(m_Wnd, strw(str).c_str());
}

void FWindowsWindow::SetTitle(const TCHAR* const Title)
{
	SetWindowText(HWnd, Title);
}

bool FWindowsWindow::isClose()
{
	return m_Close;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT FWindowsWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	FWindowsWindow* pThis = reinterpret_cast<FWindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	FInputCore::InputProc(hwnd, msg, wParam, lParam);

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return 1;
	}

	switch (msg)
	{
	case WM_CREATE:
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return DefWindowProcW(hwnd, msg, wParam, lParam);
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return DefWindowProcW(hwnd, msg, wParam, lParam);
		break;
	}

	case WM_CLOSE:
		pThis->Destroy();
		break;

	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		pThis->onResizeWindow.BroadCast(width, height);
		break;
	}

	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

FWindowsWindow* FWindowsWindowManager::CreateWindow(UINT Weight, UINT Height, FString Name)
{
	FWindowsWindow* window = new FWindowsWindow(this, (UINT)windows.GetSize());

	window->Create(Weight, Height, 0, 0, Name + to_string(windows.GetSize()), NULL);

	windows.Add(window);


	return window;

}

void FWindowsWindowManager::Update()
{
	for (auto& i : windows)
	{
		i->Update();
	}




	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			GEngine.Quit();
			return;
		}


		TranslateMessage(&msg);
		DispatchMessageW(&msg);

	}

	//if (timerIsStarted)
	//{
	//	//WaitForSingleObject(&hTimer, INFINITE);
	//	WaitForMultipleObjects(1, &hTimer, TRUE, INFINITE);
	//}

	//MsgWaitForMultipleObjects(1, &hTimer, TRUE, INFINITE, QS_ALLEVENTS);

}


void FWindowsWindowManager::SetDelay(int ms)
{
	//KillTimer(NULL, (UINT_PTR)hTimer);
	delay = ms;

	if (hTimer)
	{
		SetWaitableTimer(hTimer, &li, delay, NULL, NULL, FALSE);
		timerIsStarted = true;
	}
}



FWindowsWindowManager::FWindowsWindowManager()
{
	hTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	li.QuadPart = 0;

	delay = 1;


	if (hTimer)
	{
		//SetWaitableTimer(hTimer, &li, delay, NULL, NULL, FALSE);
	}

}

FWindowsWindow::~FWindowsWindow()
{
	this->Destroy();


}


void FWindowsWindowManager::Destroy(UINT index)
{
	auto wnd = windows[index];

	wnd->Destroy();


	delete windows[index];

	windows.Erase(windows.begin() + index);
}

void FWindowsWindowManager::Quit()
{
	for (size_t i = 0; i < windows.GetSize(); i++)
	{
		delete windows[i];
	}


	windows.Empty();
	KillTimer(NULL, (UINT_PTR)hTimer);
	timerIsStarted = false;

}


FWindowsWindowManager::~FWindowsWindowManager()
{
	Quit();

}


HRESULT FWindowsWindow::DragEnter(IDataObject* DataObject, DWORD KewState, POINTL CursorPosition, DWORD* CursorEffect)
{
	return S_OK;
}

HRESULT FWindowsWindow::DragOver(DWORD KeyState, POINTL CursorPosition, DWORD* CursorEffect)
{
	return S_OK;
}

HRESULT	FWindowsWindow::DragLeave()
{
	return S_OK;
}


HRESULT FWindowsWindow::Drop(IDataObject* DataObject, DWORD KeyState, POINTL CursorPosition, DWORD* CursorEffect)
{
	return S_OK;
}


HRESULT FWindowsWindow::QueryInterface(REFIID iid, void** ppvObject)
{
	return S_OK;
}

ULONG FWindowsWindow::AddRef()
{
	return S_OK;
}

ULONG FWindowsWindow::Release()
{
	return S_OK;
}



