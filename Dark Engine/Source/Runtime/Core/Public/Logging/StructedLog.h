#pragma once
#include "Logging/LogCategory.h"

struct FLogRecord
{
	FString Category;
	ELogVerbosity::Type Verbosity;
	FString File;
	int32 Line;
	//FTime Time;
};


