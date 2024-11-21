#pragma once
#include "Platform/Platform.h"
#include <cstdarg>
#include <cwchar>

struct FGenericString
{
	static int32 StringPrintf(TCHAR* Buffer, uint64 Count, const TCHAR* Format, va_list Args)
	{
#if PLATFORM_UTF8CHAR
		return _vsnprintf(Bufer, Count, Format, Args);
#else
		return _vsnwprintf(Buffer, Count, Format, Args);
#endif

	}

	static int32 StringToInt32(const TCHAR* Str, TCHAR** EndStr, int32 Rad)
	{
#if PLATFORM_UTF8CHAR
#else
		return wcstol(Str, EndStr, Rad);
#endif
	}

	static float StringToFloat(const TCHAR* Str, TCHAR** EndStr)
	{
#if PLATFORM_UTF8CHAR
#else
		return wcstof(Str, EndStr);
#endif
	}

	static TCHAR ToLower(const TCHAR& Ch)
	{
#if PLATFORM_UTF8CHAR
#else
		return towlower(Ch);
#endif
	}
};