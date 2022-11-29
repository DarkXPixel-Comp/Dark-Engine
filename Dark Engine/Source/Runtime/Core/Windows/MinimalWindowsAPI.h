#pragma once
#include "WindowsSystemIncludes.h"


#pragma push_macro("TRUE")
#pragma push_macro("FALSE")
//#undef TRUE
//#undef FALSE


#define MINIMAL_WINDOWS_API static

#define WINAPI __stdcall

namespace Windows
{
	// Typedefs for basic Windows types
	typedef int32_t BOOL;
	typedef unsigned char BYTE;
	typedef BYTE  BOOLEAN;
	typedef unsigned long DWORD;
	typedef DWORD* LPDWORD;
	typedef long LONG;
	typedef long* LPLONG;
	typedef int64_t LONGLONG;
	typedef LONGLONG* LPLONGLONG;
	typedef void* LPVOID;
	typedef const void* LPCVOID;
	typedef const wchar_t* LPCTSTR;

	// Typedefs for standard handles
	typedef void* HANDLE;
	typedef HINSTANCE__* HINSTANCE;
	typedef HINSTANCE HMODULE;
	typedef HWND__* HWND;
	typedef HKEY__* HKEY;
	typedef HDC__* HDC;
	typedef HICON__* HICON;
	typedef HICON__* HCURSOR;

	// Other typedefs
	//typedef tagPROCESSENTRY32W PROCESSENTRY32;
	typedef _EXCEPTION_POINTERS* LPEXCEPTION_POINTERS;
	typedef _RTL_CRITICAL_SECTION* LPCRITICAL_SECTION;
	typedef _OVERLAPPED* LPOVERLAPPED;
	typedef _LARGE_INTEGER* LPLARGE_INTEGER;

	typedef _RTL_SRWLOCK RTL_SRWLOCK, * PRTL_SRWLOCK;
	typedef RTL_SRWLOCK* PSRWLOCK;

	// Opaque SRWLOCK structure
	struct SRWLOCK
	{
		void* Ptr;
	};

	// Constants
	/*static const BOOL TRUE = 1;
	static const BOOL FALSE = 0;*/

	
	//
	

}