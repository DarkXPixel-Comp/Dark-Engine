#include "Misc/GlobalLogger.h"
#include "Misc/ScopeLock.h"
#include "Misc/OutputDevice.h"

FGlobalLogger::FGlobalLogger()
{
}

CORE_API FGlobalLogger& FGlobalLogger::Get()
{
	static FGlobalLogger Logger;
	return Logger;
}

CORE_API void FGlobalLogger::AddOutputDevice(FOutputDevice* OutputDevice)
{
	FScopeLock ScopeLock(&CriticalSection);
	OutputDevices.Add(OutputDevice);
}

CORE_API void FGlobalLogger::RemoveOutputDevice(FOutputDevice* OutputDevice)
{
	FScopeLock ScopeLock(&CriticalSection);
	OutputDevices.Remove(OutputDevice);
}

CORE_API void FGlobalLogger::LogRecord(const FLogRecord& Record)
{
	FScopeLock ScopeLock(&CriticalSection);

	for (FOutputDevice* OutputDevice : OutputDevices)
	{
		OutputDevice->LogRecord(Record);
	}
}

CORE_API void FGlobalLogger::Log(const TCHAR* Data, ELogVerbosity::Type Verbosity, const FString& Category, const double Time)
{
	FScopeLock ScopeLock(&CriticalSection);

	for (FOutputDevice* OutputDevice : OutputDevices)
	{
		OutputDevice->Log(Data, Verbosity, Category, Time);
	}
}
