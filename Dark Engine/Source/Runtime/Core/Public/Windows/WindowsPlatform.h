#pragma once
#include "GenericPlatform/GenericPlatform.h"			

#if defined(__clang__)
#else
#include "MSVC/MSVCPlatform.h"
#endif




struct FWindowsPlatformTypes : public FGenericPlatformTypes
{
#ifdef _WIN64
	typedef unsigned __int64 SIZE_T;
	typedef __int64 SSIZE_T;
#else
	typedef unsigned long SIZE_T;
	typedef long SSIZE_T;
#endif // _WIN64
#if USE_UTF8_TCHARS
	typedef UTF8CHAR TCHAR;
#endif
};


typedef FWindowsPlatformTypes FPlatformTypes;





// Base defines, must define these for the platform, there are no defaults
#define PLATFORM_DESKTOP					1
#if defined( _WIN64 )
#define PLATFORM_64BITS					1
#else
#define PLATFORM_64BITS					0
#endif
#if defined( _MANAGED ) || defined( _M_CEE )
#define PLATFORM_COMPILER_COMMON_LANGUAGE_RUNTIME_COMPILATION	1
#endif
#define PLATFORM_CAN_SUPPORT_EDITORONLY_DATA				1

// Base defines, defaults are commented out

#define PLATFORM_LITTLE_ENDIAN								1
#define PLATFORM_SUPPORTS_UNALIGNED_LOADS					1

#define PLATFORM_SUPPORTS_PRAGMA_PACK						1
#if defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)
#define PLATFORM_CPU_ARM_FAMILY							1
#define PLATFORM_ENABLE_VECTORINTRINSICS_NEON			1
#define PLATFORM_ENABLE_VECTORINTRINSICS				1
#elif (defined(_M_IX86) || defined(_M_X64))
#define PLATFORM_CPU_X86_FAMILY							1
#define PLATFORM_ENABLE_VECTORINTRINSICS				1

#endif
// FMA3 support was added starting from AMD Piledriver (excluding Jaguar) and Intel Haswell (excluding Pentium and Celeron)
#ifndef PLATFORM_ALWAYS_HAS_FMA3
#define PLATFORM_ALWAYS_HAS_FMA3						0
#endif

#define PLATFORM_USE_LS_SPEC_FOR_WIDECHAR					0
//#define PLATFORM_USE_SYSTEM_VSWPRINTF						1
//#define PLATFORM_TCHAR_IS_4_BYTES							0
#define PLATFORM_HAS_BSD_TIME								0
#define PLATFORM_USE_PTHREADS								0
#define PLATFORM_USES_UNFAIR_LOCKS							1
#define PLATFORM_MAX_FILEPATH_LENGTH_DEPRECATED				WINDOWS_MAX_PATH
#define PLATFORM_HAS_BSD_IPV6_SOCKETS						1
#define PLATFORM_HAS_BSD_SOCKET_FEATURE_WINSOCKETS			1
#define PLATFORM_USES_MICROSOFT_LIBC_FUNCTIONS				1
#define PLATFORM_IS_ANSI_MALLOC_THREADSAFE					1
#if PLATFORM_CPU_ARM_FAMILY
#define PLATFORM_SUPPORTS_TBB								0
#else
#define PLATFORM_SUPPORTS_TBB								1
#endif
#define PLATFORM_SUPPORTS_MIMALLOC							PLATFORM_64BITS
#define PLATFORM_SUPPORTS_NAMED_PIPES						1
#define PLATFORM_COMPILER_HAS_TCHAR_WMAIN					1
#define PLATFORM_SUPPORTS_EARLY_MOVIE_PLAYBACK				(!WITH_EDITOR) // movies will start before engine is initalized
#define PLATFORM_USE_GENERIC_STRING_IMPLEMENTATION			0 // Can set this to 1 if you need to debug FGenericWidePlatformString::GetVarArgs on Windows
#define PLATFORM_SUPPORTS_VIRTUAL_TEXTURE_STREAMING			1
#define PLATFORM_SUPPORTS_VARIABLE_RATE_SHADING				1
#define PLATFORM_SUPPORTS_MESH_SHADERS						1
#define PLATFORM_SUPPORTS_BINDLESS_RENDERING				1
#define PLATFORM_USES__ALIGNED_MALLOC						1

#if WITH_EDITOR
#define PLATFORM_FILE_READER_BUFFER_SIZE					(256*1024)
#endif

#define PLATFORM_SUPPORTS_STACK_SYMBOLS						1
#define PLATFORM_COMPILER_HAS_DECLTYPE_AUTO					1

#define PLATFORM_GLOBAL_LOG_CATEGORY						LogWindows

#define PLATFORM_SUPPORTS_BORDERLESS_WINDOW					1

#define PLATFORM_RETURN_ADDRESS_FOR_CALLSTACKTRACING		PLATFORM_RETURN_ADDRESS_POINTER

#define WINDOWS_USE_FEATURE_APPLICATIONMISC_CLASS			1
#define WINDOWS_USE_FEATURE_PLATFORMMISC_CLASS				1
#define WINDOWS_USE_FEATURE_PLATFORMHTTP_CLASS				1


// Q: Why is there a __nop() before __debugbreak()?
// A: VS' debug engine has a bug where it will silently swallow explicit
// breakpoint interrupts when single-step debugging either line-by-line or
// over call instructions. This can hide legitimate reasons to trap. Asserts
// for example, which can appear as if the did not fire, leaving a programmer
// unknowingly debugging an undefined process.
#define PLATFORM_BREAK() (__nop(), __debugbreak())

// Intrinsics for 128-bit atomics on Windows platform requires Windows 8 or higher (WINVER>=0x0602)
// http://msdn.microsoft.com/en-us/library/windows/desktop/hh972640.aspx
#define PLATFORM_HAS_128BIT_ATOMICS							(PLATFORM_64BITS && (WINVER >= 0x602))



#define VARARGS     __cdecl											/* Functions with variable arguments */
#define CDECL	    __cdecl											/* Standard C function */
#define STDCALL		__stdcall										/* Standard calling convention */
#define FORCEINLINE __forceinline									/* Force code to be inline */
#define FORCENOINLINE __declspec(noinline)							/* Force code to NOT be inline */
#define FUNCTION_NON_NULL_RETURN_START _Ret_notnull_				/* Indicate that the function never returns nullptr. */
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#define FORCEINLINE __forceinline