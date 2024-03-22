#include "Misc/AssertionMacros.h"
#include <cstdarg>

#include "Logging/LogMacros.h"


DECLARE_LOG_CATEGORY(AssertMacros, Display);


bool VARARGS FDebug::CheckVerifyFailedImpl(
	const ANSICHAR* Expr,
	const ANSICHAR* File,
	int32 Line,
	void* ProgramCounter,
	const TCHAR* Format,
	...)
{
	DE_LOG(AssertMacros, Display, TEXT("%s in %s(%i)"), *FString(Expr), *FString(File), Line);

	
	return true;
}

void FDebug::LogAssertFailedMessageImplV(const ANSICHAR* Expr, const ANSICHAR* File, int32 Line, void* ProgramCounter, const TCHAR* Fmt, va_list Args)
{
	//if (!GIsCriticalError)
	{
		//StaticFailDebugV(TEXT("AssertionFailed: "), Expr, File, Line, false, ProgramCounter, Fmt, Args);
	}

}
