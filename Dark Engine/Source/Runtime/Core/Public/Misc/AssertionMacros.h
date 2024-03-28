#pragma once
#include "HAL/PreprocessorHelpers.h"
#include "CoreTypes.h"




namespace ELogVerbosity
{
	enum Type : uint8;
}


struct FEnsureHandlerArgs
{
	const ANSICHAR* Expression;
	const TCHAR* Message;
};

template <typename FuncType> class TFunction;

DENGINE_API	TFunction<bool(const FEnsureHandlerArgs& Args)> SetEnsureHandler(TFunction<bool(const FEnsureHandlerArgs& Args)> EnsureHandler);

DENGINE_API	TFunction<bool(const FEnsureHandlerArgs& Args)> GetEnsureHandler();





struct FDebug
{
public:
	static DENGINE_API bool VARARGS	CheckVerifyFailedImpl(const ANSICHAR* Expr, const ANSICHAR* File, int32 Line, void* ProgramCounter, const TCHAR* Format, ...);
	static DENGINE_API void LogAssertFailedMessageImplV(const ANSICHAR* Expr, const ANSICHAR* File, int32 Line, void* ProgramCounter, const TCHAR* Fmt, va_list Args);



};







#ifdef DO_CHECK
#ifndef checkCode
#define checkCode( Code )		do { Code; } while ( false );
#endif
#ifndef verify
#define verify(expr)			DE_CHECK_IMPL(expr)
#endif
#ifndef check
#define check(expr)				DE_CHECK_IMPL(expr)
#ifndef	checkSlow
#define checkSlow(expr)			check(expr)
#endif

#endif

// Technically we could use just the _F version (lambda-based) for asserts
// both with and without formatted messages. However MSVC emits extra
// unnecessary instructions when using a lambda; hence the Exec() impl.
#define DE_CHECK_IMPL(expr) \
		{ \
			if(UNLIKELY(!(expr))) \
			{ \
				if (FDebug::CheckVerifyFailedImpl(#expr, __FILE__, __LINE__, PLATFORM_RETURN_ADDRESS(), TEXT(""))) \
				{ \
					if(IsDebuggerPresent())\
						{PLATFORM_BREAK();} \
				} \
			} \
		}

	/**
	 * verifyf, checkf: Same as verify, check but with printf style additional parameters
	 * Read about __VA_ARGS__ (variadic macros) on http://gcc.gnu.org/onlinedocs/gcc-3.4.4/cpp.pdf.
	 */
#ifndef verifyf
#define verifyf(expr, format,  ...)		DE_CHECK_F_IMPL(expr, format, ##__VA_ARGS__)
#endif
#ifndef checkf
#define checkf(expr, format,  ...)		DE_CHECK_F_IMPL(expr, format, ##__VA_ARGS__)
#endif

#define DE_CHECK_F_IMPL(expr, format, ...) \
		{ \
			if(UNLIKELY(!(expr))) \
			{ \
				if (FDebug::CheckVerifyFailedImpl(#expr, __FILE__, __LINE__, PLATFORM_RETURN_ADDRESS(), format, ##__VA_ARGS__)) \
				{ \
					PLATFORM_BREAK(); \
				} \
			} \
		}

	 /**
	  * Denotes code paths that should never be reached.
	  */
#ifndef checkNoEntry
#define checkNoEntry()       check(!"Enclosing block should never be called")
#endif

	  /**
	   * Denotes code paths that should not be executed more than once.
	   */
#ifndef checkNoReentry
#define checkNoReentry()     { static bool s_beenHere##__LINE__ = false;                                         \
	                               check( !"Enclosing block was called more than once" || !s_beenHere##__LINE__ );   \
								   s_beenHere##__LINE__ = trDE; }
#endif

class FRecursionScopeMarker
{
public:
	FRecursionScopeMarker(uint16& InCounter) : Counter(InCounter) { ++Counter; }
	~FRecursionScopeMarker() { --Counter; }
private:
	uint16& Counter;
};

/**
 * Denotes code paths that should never be called recursively.
 */
#ifndef checkNoRecursion
#define checkNoRecursion()  static uint16 RecursionCounter##__LINE__ = 0;                                            \
	                            check( !"Enclosing block was entered recursively" || RecursionCounter##__LINE__ == 0 );  \
	                            const FRecursionScopeMarker ScopeMarker##__LINE__( RecursionCounter##__LINE__ )
#endif

#ifndef unimplemented
#define unimplemented()		check(!"Unimplemented function called")
#endif

#else
#define checkCode(...)
#define check(expr)					{ }
#define checkf(expr, format, ...)	{ }
#define checkNoEntry()
#define checkNoReentry()
#define checkNoRecursion()
#define verify(expr)				{ if(UNLIKELY(!(expr))){  } }
#define verifyf(expr, format, ...)	{ if(UNLIKELY(!(expr))){  } }
#define unimplemented()				{  }
#endif
