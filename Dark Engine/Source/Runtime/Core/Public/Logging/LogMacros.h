#pragma once
#include "Logging/LogCategory.h"

#include <format>
#include <Logger.h>


struct FMsg
{
	static void LogV(const ANSICHAR* File, int32 Line, const FString& Category, ELogVerbosity::Type Verbosity,
		const TCHAR* Fmt, va_list Args)
	{
		//if (Verbosity != ELogVerbosity::Fatal)
		{
			FString Message;
			Message.AppendV(Fmt, Args);

			log_ CurrentLog;
			FString Verbose = TEXT("");

			switch (Verbosity)
			{
			case ELogVerbosity::Fatal:
				//Log.Color = { 1.f, 0.f, 0.f };
				Verbose = TEXT("FATAL");
				break;
			case ELogVerbosity::Error:
				//Log.Color = { 0.8f, 0.f, 0.f };
				Verbose = TEXT("Error");
				break;
			case ELogVerbosity::Warning:
				//Log.Color = { 0.7f, 0.7f, 0.f };
				Verbose = TEXT("Warning");
				break;
			case ELogVerbosity::Display:
				//Log.Color = { 0.1f, 0.1f, 0.1f };
				Verbose = TEXT("Display");
				break;
			case ELogVerbosity::Log:
				//Log.Color = { 0.1f, 0.1f, 0.1f };
				Verbose = TEXT("Log");
				break;
			default:
				break;
			}

			/*FString Result;
			Result.AppendV(TEXT("%S: %S: %S"), L"TEST", L"TEST2", L"TEST3")*/

			//FString Result = FString::PrintF(TEXT("%i: %s: %s"), 25, L"TEST2", L"TEST3");
			FString Result = FString::PrintF(TEXT("[%s] [%s]: %s"), *Category, *Verbose, *Message);
			CurrentLog.ModernLog = true;
			CurrentLog.Result = Result;
			Logger::log(CurrentLog);

			/*FString Result = std::format(TEXT("{}: {}: {}"), *Category, *Verbose, Message);
			Log.ModernLog = true;
			Log.Result = Result;
			Logger::log(Log);*/
		}
	}
}; 





struct FStaticBasicLogRecord
{
	const TCHAR* Format = nullptr;
	const ANSICHAR* File = nullptr;
	int32 Line = 0;
	ELogVerbosity::Type Verbosity = ELogVerbosity::Log;

	constexpr FStaticBasicLogRecord
	(
		const TCHAR* InFormat,
		const ANSICHAR* InFile,
		int32 InLine,
		ELogVerbosity::Type InVebosity
	):
		Format(InFormat),
		File(InFile),
		Line(InLine),
		Verbosity(InVebosity)
	{}

};




void BasicLog(const FLogCategoryBase& Category, const FStaticBasicLogRecord* Log, ...);



#define DE_LOG(CategoryName, Verbosity, Format, ...) \
{	\
	static constexpr FStaticBasicLogRecord LOG_Static(Format, __builtin_FILE(), __builtin_LINE(), ELogVerbosity::Verbosity)	;\
	if((ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) == ELogVerbosity::Fatal) \
	{ }		\
	else if constexpr ((ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) <= ELogVerbosity::VeryVerbose)	 \
	{	 \
		BasicLog(CategoryName, &LOG_Static, ##__VA_ARGS__);   \
	}\
}





#define DECLARE_LOG_CATEGORY(CategoryName, DefaultVerbosity) \
static struct FLogCategory##CategoryName : public FLogCategory<ELogVerbosity::DefaultVerbosity> \
	{									  \
			FORCEINLINE FLogCategory##CategoryName() : FLogCategory(TEXT(#CategoryName)) {}\
	} CategoryName; 										  \


