#pragma once
#include "Misc/OutputDevice.h"
#include "Platform/Platform.h"


class FOutputDeviceDebug : public FOutputDevice
{
public:
	CORE_API virtual void LogRecord(const FLogRecord&) override;
	CORE_API virtual void Log(const TCHAR* Data, ELogVerbosity::Type Verbosity, const FString& Category, const double Time) {};

	CORE_API static FOutputDevice* Get();

};
