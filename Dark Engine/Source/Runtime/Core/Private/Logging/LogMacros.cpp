#include "Logging/LogMacros.h"
#include <thread>
#include <mutex>
#include <fstream>
#include "Memory/MemoryCore.h"
#include "Misc/Paths.h"
#include "Logging/StructedLog.h"
#include "Misc/GlobalLogger.h"


void FMsg::LogV(const ANSICHAR* File, int32 Line, const FString& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, va_list Args)
{
	FLogRecord Record;
	FString Message;
	Message.AppendV(Fmt, Args);

	Record.SetCategory(Category);
	Record.SetFile(File);
	Record.SetTime(std::chrono::system_clock::now());
	Record.SetLine(Line);
	Record.SetVerbosity(Verbosity);
	Record.SetFormat(*Message);

	FGlobalLogger::Get().LogRecord(Record);
}

void BasicLog(const FLogCategoryBase& Category, const FStaticBasicLogRecord* Log, ...)
{
	va_list Args;
	va_start(Args, Log);
	FMsg::LogV(Log->File, Log->Line, Category.GetCategoryName(), Log->Verbosity,
		static_cast<const wchar_t*>(Log->Format), Args);
	va_end(Args);

}