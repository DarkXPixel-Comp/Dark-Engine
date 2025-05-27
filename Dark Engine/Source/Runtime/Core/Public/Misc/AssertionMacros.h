#pragma once
#include "Platform/Platform.h"
#include "Containers/DarkString.h"

extern bool GIsCriticalError;

class FDebug
{
public:
	static CORE_API void __cdecl AssertFailed(const ANSICHAR* Expression, const ANSICHAR* File, int32 Line, const TCHAR* Format = TEXT(""), ...);
	static CORE_API void AssertFailedV(const ANSICHAR* Expression, const ANSICHAR* File, int32 Line, const TCHAR* Format, va_list Args);

	static CORE_API bool __cdecl CheckFailed(const ANSICHAR* Expression, const ANSICHAR* File, int32 Line, const TCHAR* Format = TEXT(""), ...);


	static CORE_API void  __cdecl OpenHTMLError(const ANSICHAR* Expression, const ANSICHAR* File, int32 Line, const TCHAR* Format = TEXT(""), ...);
	static CORE_API void OpenHTMLErrorV(const ANSICHAR* Expression, const ANSICHAR* File, int32 Line, const TCHAR* Format, va_list Args);

	static CORE_API void OpenHTMLCallStack(const FString& ExceptionCode, const FString& ExceptionAddress, const FString& StackTrace);

};



#ifdef _DEBUG
#define check(expr)				DE_CHECK_IMPL(expr)

#define DE_CHECK_IMPL(expr)  \
	if(!expr) \
	{ \
		if(FDebug::CheckFailed(#expr, __FILE__, __LINE__, TEXT(""))) \
		{		  \
			PLATFORM_BREAK(); \
		} \
	} 
#else
#define check(expr)
#endif