#include "Misc/OutputDeviceStd.h"
#include "Logging/StructedLog.h"


#if WIN32
static bool IsStdOutAttachedToConsole()
{
	HANDLE StdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (StdoutHandle != INVALID_HANDLE_VALUE)
	{
		DWORD FileType = GetFileType(StdoutHandle);
		if (FileType == FILE_TYPE_CHAR)
		{
			return true;
		}
	}
	return false;
}
#endif

FOutputDeviceStd::FOutputDeviceStd()
{
#if WIN32
	bIsConsoleOutput = IsStdOutAttachedToConsole();
#endif

}

CORE_API void FOutputDeviceStd::LogRecord(const FLogRecord& Record)
{
	FString String;
	FormatLogLine(String, Record);
	String.Append(TEXT("\r\n"));

#if WIN32
	if (bIsConsoleOutput)
	{
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), *String, String.Lenght(), nullptr, nullptr);
		return;
	}
#endif

	wprintf(TEXT("%s"), *String);
	fflush(stdout);
}		  

CORE_API void FOutputDeviceStd::Log(const TCHAR* Data, ELogVerbosity::Type Verbosity, const FString& Category, const double Time)
{




}
