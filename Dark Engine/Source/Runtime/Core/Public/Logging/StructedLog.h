#pragma once
#include "Logging/LogCategory.h"

class FLogRecord
{
public:
	const FString& GetCategory() const { return Category; }
	void SetCategory(const FString& InCategory) { Category = InCategory; }
	void SetFile(const ANSICHAR* InFile) { File = InFile; }
	void SetLine(int32 InLine) { Line = InLine; }
	void SetVerbosity(ELogVerbosity::Type InVerbosity) { Verbosity = InVerbosity; }
	void SetTime(std::chrono::system_clock::time_point InTime) { Time = InTime; }
	void SetFormat(const TCHAR* InFormat) { Format = InFormat; }
	const TCHAR* GetFormat() const { return Format; }
	std::chrono::system_clock::time_point GetTime() const { return Time; }
	const ANSICHAR* GetFile() const { return File; }
	int32 GetLine() const { return Line; }
	ELogVerbosity::Type GetVerbosity() const { return Verbosity; }

private:
	const TCHAR* Format = nullptr;
	const ANSICHAR* File = nullptr;
	FString Category;
	ELogVerbosity::Type Verbosity = ELogVerbosity::Log;
	std::chrono::system_clock::time_point Time;
	int32 Line = 0;
};