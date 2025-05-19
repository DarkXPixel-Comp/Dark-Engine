#pragma once
#include "Platform/Platform.h"
#include "Containers/Array.h"
#include "Platform/PlatformCriticalSection.h"
#include "Logging/LogCategory.h"
#include <thread>


class FOutputDevice;

class FGlobalLogger
{
public:
	CORE_API FGlobalLogger();
	static CORE_API FGlobalLogger& Get();

	CORE_API void AddOutputDevice(FOutputDevice* OutputDevice);
	CORE_API void RemoveOutputDevice(FOutputDevice* OutputDevice);

	CORE_API void LogRecord(const class FLogRecord& Record);
	CORE_API void Log(const TCHAR* Data, ELogVerbosity::Type Verbosity, const FString& Category, const double Time);



private:
	TArray<FOutputDevice*> OutputDevices;
	FCriticalSection CriticalSection;
	std::thread Thread;
};