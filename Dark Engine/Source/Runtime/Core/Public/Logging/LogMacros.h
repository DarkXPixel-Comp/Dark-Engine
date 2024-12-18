#pragma once

#include "Logging/LogCategory.h"

#include <format>

struct FMsg
{
	static void LogV(const ANSICHAR* File, int32 Line, const FString& Category, ELogVerbosity Verbosity, const TCHAR* Fmt, va_list Args)
	{
		FString Message;
		Message.AppendV(Fmt, Args);

		FString Verbose;

		switch (Verbosity)
		{
		case ELogVerbosity::Fatal:
			Verbose = TEXT("FATAL");
			break;
		case ELogVerbosity::Error:
			Verbose = TEXT("Error");
			break;
		case ELogVerbosity::Warning:
			Verbose = TEXT("Warning");
			break;
		case ELogVerbosity::Log:
			Verbose = TEXT("Log");
			break;
		case ELogVerbosity::Console:
			Verbose = TEXT("Log");
			break;
		default:
			return;
		}

		FString Result = FString::PrintF(TEXT("[%s] [%s]: %s"), *Category, *Verbose, *Message);

	}
};