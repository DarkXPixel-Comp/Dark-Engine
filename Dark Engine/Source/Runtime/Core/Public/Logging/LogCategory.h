#pragma once

#include "Containers/DarkString.h"

enum class ELogVerbosity : uint8
{
	NoLogging = 1 << 0,

	Fatal = 1 << 1,

	Error = 1 << 2,

	Warning = 1 << 3,

	Log = 1 << 4,

	Console = 1 << 5
};

struct CORE_API FLogCategoryBase
{
	FLogCategoryBase(const FString& InCategoryName, ELogVerbosity InVerbosity) :
		CategoryName(InCategoryName),
		Verbosity(InVerbosity)
	{}


private:
	ELogVerbosity Verbosity;
	ELogVerbosity DefaultVerbosity;
	bool DebugBreakOnLog;
	ELogVerbosity DebugBreakOnLogVerbosity;
	const FString CategoryName;
};
