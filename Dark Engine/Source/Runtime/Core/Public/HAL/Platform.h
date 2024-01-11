#pragma once
//#include "Definitions.h"
#include "PreprocessorHelpers.h"


#if !defined(PLATFORM_WINDOWS)
#define PLATFORRM_WINDOWS 0
#endif
#if !defined(PLATFORM_MICROSOFT)
#define PLATFORM_MICROSOFT 0
#endif

#include COMPILED_PLATFORM_HEADER(Platform.h)



#ifndef LIKELY						/* Hints compiler that expression is likely to be true, much softer than UE_ASSUME - allows (penalized by worse performance) expression to be false */
#if ( defined(__clang__) || defined(__GNUC__) ) && (PLATFORM_UNIX)	// effect of these on non-Linux platform has not been analyzed as of 2016-03-21
#define LIKELY(x)			__builtin_expect(!!(x), 1)
#else
// the additional "!!" is added to silence "warning: equality comparison with exteraneous parenthese" messages on android
#define LIKELY(x)			(!!(x))
#endif
#endif

#ifndef UNLIKELY					/* Hints compiler that expression is unlikely to be true, allows (penalized by worse performance) expression to be true */
#if ( defined(__clang__) || defined(__GNUC__) ) && (PLATFORM_UNIX)	// effect of these on non-Linux platform has not been analyzed as of 2016-03-21
#define UNLIKELY(x)			__builtin_expect(!!(x), 0)
#else
	// the additional "!!" is added to silence "warning: equality comparison with exteraneous parenthese" messages on android
#define UNLIKELY(x)			(!!(x))
#endif
#endif


typedef FPlatformTypes::int8 int8;
typedef FPlatformTypes::uint8 uint8;
typedef FPlatformTypes::int16 int16;
typedef FPlatformTypes::uint16 uint16;
typedef FPlatformTypes::int32 int32;
typedef FPlatformTypes::uint32 uint32;
typedef FPlatformTypes::int64 int64;
typedef FPlatformTypes::uint64 uint64;
typedef FPlatformTypes::ANSICHAR ANSICHAR;
typedef FPlatformTypes::WIDECHAR WIDECHAR;
typedef FPlatformTypes::TCHAR TCHAR;
typedef FPlatformTypes::UTF8CHAR UTF8CHAR;
typedef FPlatformTypes::CHAR16 UCS2CHAR;
typedef FPlatformTypes::CHAR16 UTF16CHAR;
typedef FPlatformTypes::CHAR32 UTF32CHAR;

typedef FPlatformTypes::SIZE_T SIZE_T;
typedef FPlatformTypes::SSIZE_T SSIZE_T;


#define UTF8TEXT_PASTE(x) u8 ## x
#define UTF16TEXT_PASTE(x) u ## x
#define WIDETEXT_PASTE(x) L ## x

#if !defined(TEXT)
#if PLATFORM_TCHAR_IS_UTF8CHAR
#define TEXT_PASTE(x) UTF8TEXT(x)
#else
#define TEXT_PASTE(x) WIDETEXT(x)
#endif
#define TEXT(x) TEXT_PASTE(x)
#endif




#define UTF16TEXT(x) UTF16TEXT_PASTE(x)
#define WIDETEXT(str) WIDETEXT_PASTE(str)

