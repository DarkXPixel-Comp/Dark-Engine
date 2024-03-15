#include "Logging/LogMacros.h"
//#include "Logger.h"
#include <cstdarg>



void BasicLog(const FLogCategoryBase& Category, const FStaticBasicLogRecord* Log, ...)
{
	va_list	 Args;
	va_start(Args, Log);
	//FString test = Log->Format;
	FMsg::LogV(Log->File, Log->Line, Category.GetCategoryName(), Log->Verbosity,
		static_cast<const wchar_t*>(Log->Format), Args);
	va_end(Args);





}