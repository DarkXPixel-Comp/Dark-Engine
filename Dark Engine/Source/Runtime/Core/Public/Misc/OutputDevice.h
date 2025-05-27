#pragma once

#include "Platform/Platform.h"
#include "Logging/StructedLog.h"


class FOutputDevice
{
public:
	CORE_API virtual void LogRecord(const FLogRecord& Record) = 0;

	CORE_API virtual void Log(const TCHAR* Data, ELogVerbosity::Type Verbosity, const FString& Category, const double Time) = 0;

	CORE_API static void FormatLogLine(FString& Format, const FLogRecord& Record);

	CORE_API static FOutputDevice* Get() { return nullptr; }


};