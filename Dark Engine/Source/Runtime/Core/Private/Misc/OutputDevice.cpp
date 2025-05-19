#include <iomanip>
#include <sstream>
#include "Misc/OutputDevice.h"
#include <ctime>

CORE_API void FOutputDevice::FormatLogLine(FString& Format, const FLogRecord& Record)
{
    // Convert std::chrono::time_point to a string
    auto Time = Record.GetTime();
    std::time_t TimeT = std::chrono::system_clock::to_time_t(Time);
    std::tm LocalTime;
    localtime_s(&LocalTime, &TimeT); // Thread-safe on Windows
    std::ostringstream TimeStream;
    TimeStream << std::put_time(&LocalTime, "%Y-%m-%d %H:%M:%S");

    // Append the formatted time to the FString
    Format.Append(TEXT("%s"), *FString(TimeStream.str().c_str()));


	Format.Append(TEXT(" %s: "), *Record.GetCategory());
	if (Record.GetVerbosity() != ELogVerbosity::Log)
	{
		Format.Append(TEXT("%s"), *FString(ELogVerbosity::ToString(Record.GetVerbosity())));
        Format.Append(TEXT(": "));
	}

	if (Record.GetFormat())
	{
		Format.Append(TEXT("%s"), Record.GetFormat());
	}

}
