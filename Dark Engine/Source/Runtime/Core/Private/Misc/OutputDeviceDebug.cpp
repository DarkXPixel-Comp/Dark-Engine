#include "Misc/OutputDeviceDebug.h"
#include "Containers/DarkString.h"

CORE_API void FOutputDeviceDebug::LogRecord(const FLogRecord& Record)
{
	FString String;
	FormatLogLine(String, Record);
	String.Append(TEXT("\r\n"));
	OutputDebugString(*String);
}

CORE_API FOutputDevice* FOutputDeviceDebug::Get()
{
	static FOutputDeviceDebug Inst;
	return &Inst;
}
