#include "Logging/LogMacros.h"


//void FMsg::LogV(const ANSICHAR* File, int32 Line, const FString& Category, ELogVerbosity::Type Verbosity,
//	const TCHAR* Fmt, va_list Args)
//{
//	//if (Verbosity != ELogVerbosity::Fatal)
//	{
//		FString Message;
//		//Message.AppendV(Fmt, Args);
//
//		//FString Message = std::format(Fmt, Args...);
//		log_ Log;
//		FString Verbose = TEXT("");
//
//		switch (Verbosity)
//		{
//		case ELogVerbosity::Fatal:
//			//Log.Color = { 1.f, 0.f, 0.f };
//			Verbose = TEXT("FATAL");
//			break;
//		case ELogVerbosity::Error:
//			//Log.Color = { 0.8f, 0.f, 0.f };
//			Verbose = TEXT("Error");
//			break;
//		case ELogVerbosity::Warning:
//			//Log.Color = { 0.7f, 0.7f, 0.f };
//			Verbose = TEXT("Warning");
//			break;
//		case ELogVerbosity::Display:
//			//Log.Color = { 0.1f, 0.1f, 0.1f };
//			Verbose = TEXT("Display");
//			break;
//		case ELogVerbosity::Log:
//			//Log.Color = { 0.1f, 0.1f, 0.1f };
//			Verbose = TEXT("Log");
//			break;
//		default:
//			break;
//		}
//
//
//
//		/*FString Result = std::format(TEXT("{}: {}: {}"), *Category, *Verbose, Message);
//		Log.ModernLog = true;
//		Log.Result = Result;
//		Logger::log(Log);*/
//	}
//}