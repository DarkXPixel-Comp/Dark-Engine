#pragma once
#include <Windows.h>
#include <HAL/Platform.h>


namespace Windows
{
	// Typedefs for basic Windows types
	typedef int32 BOOL;
	typedef unsigned char BYTE;
	typedef BYTE  BOOLEAN;
	typedef unsigned long DWORD;
	typedef DWORD* LPDWORD;
	typedef long LONG;
	typedef long* LPLONG;
	typedef int64 LONGLONG;
	typedef LONGLONG* LPLONGLONG;
	typedef void* LPVOID;
	typedef const void* LPCVOID;
	typedef const wchar_t* LPCTSTR;

	// Typedefs for standard handles
	typedef void* HANDLE;
	typedef HINSTANCE__* HINSTANCE;
	typedef HINSTANCE HMODULE;
	//typedef HWND__* HWND;
	//typedef HKEY__* HKEY;
	//typedef HDC__* HDC;
	//typedef HICON__* HICON;
	//typedef HICON__* HCURSOR;
}