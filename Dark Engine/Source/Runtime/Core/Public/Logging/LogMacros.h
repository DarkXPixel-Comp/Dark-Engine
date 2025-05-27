#pragma once
#include "Logging/LogCategory.h"
#include <format>




struct FMsg
{
	static void LogV(const ANSICHAR* File, int32 Line, const FString& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, va_list Args);
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
	) :
		Format(InFormat),
		File(InFile),
		Line(InLine),
		Verbosity(InVebosity)
	{
	}

};

CORE_API void BasicLog(const FLogCategoryBase& Category, const FStaticBasicLogRecord* Log, ...);

#define DE_LOG(CategoryName, Verbosity, Format, ...) \
{	\
	static constexpr FStaticBasicLogRecord LOG_Static(Format, __builtin_FILE(), __builtin_LINE(), ELogVerbosity::Verbosity)	;\
	if constexpr ((ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) == ELogVerbosity::Fatal) \
	{	  \
		BasicLog(CategoryName, &LOG_Static, ##__VA_ARGS__);		\
	}		\
	else if constexpr ((ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) <= ELogVerbosity::VeryVerbose)	 \
	{	 \
		BasicLog(CategoryName, &LOG_Static, ##__VA_ARGS__);   \
	}\
	else {BasicLog(CategoryName, &LOG_Static, ##__VA_ARGS__);}\
}





#define DECLARE_LOG_CATEGORY(CategoryName, DefaultVerbosity) \
static struct FLogCategory##CategoryName : public FLogCategory<ELogVerbosity::DefaultVerbosity> \
	{									  \
			FORCEINLINE FLogCategory##CategoryName() : FLogCategory(TEXT(#CategoryName)) {}\
	} CategoryName;
