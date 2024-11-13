#pragma once
#include <cstdint>

struct FGenericPlatformTypes
{
	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef int16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;

	typedef char ANSICHAR;
	typedef wchar_t WIDECHAR;

	typedef unsigned char UTF8CHAR;

	typedef uint8 CHAR8;
	typedef uint16 CHAR16;
	typedef uint32 CHAR32;
};