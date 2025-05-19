#pragma once
#include "OutputDevice.h"
#include <iostream>


class FOutputDeviceStd : public FOutputDevice
{
public:
	CORE_API FOutputDeviceStd();

	CORE_API virtual void LogRecord(const FLogRecord& Record) override;
	CORE_API virtual void Log(const TCHAR* Data, ELogVerbosity::Type Verbosity, const FString& Category, const double Time) override;
	static CORE_API FOutputDevice* Get();

private:
	bool bIsConsoleOutput = false;
};

