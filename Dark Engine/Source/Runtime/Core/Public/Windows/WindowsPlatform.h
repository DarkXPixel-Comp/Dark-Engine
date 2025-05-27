#pragma once
#include "GenericPlatform/GenericPlatform.h"
#include "Windows/WindowsIncludes.h"
#include <intrin.h>

struct FWindowsPlatformTypes : public FGenericPlatformTypes
{
	typedef WIDECHAR TCHAR;
};

typedef FWindowsPlatformTypes FPlatformTypes;


#define PLATFORM_BREAK() (__nop(), __debugbreak())

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)
