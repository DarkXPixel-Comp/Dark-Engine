#include "Misc/AssertionMacros.h"
#include <cstdarg>


bool VARARGS FDebug::CheckVerifyFailedImpl(
	const ANSICHAR* Expr,
	const ANSICHAR* File,
	int32 Line,
	void* ProgramCounter,
	const TCHAR* Format,
	...)
{
	va_list	Args;

	va_start(Args, Format);
	FDebug::LogAssertFailedMessageImplV(Expr, File, Line, ProgramCounter, Format, Args);

	va_end(Args);

	return true;
}

void FDebug::LogAssertFailedMessageImplV(const ANSICHAR* Expr, const ANSICHAR* File, int32 Line, void* ProgramCounter, const TCHAR* Fmt, va_list Args)
{
	//if (!GIsCriticalError)
	{
		//StaticFailDebugV(TEXT("AssertionFailed: "), Expr, File, Line, false, ProgramCounter, Fmt, Args);
	}

}
