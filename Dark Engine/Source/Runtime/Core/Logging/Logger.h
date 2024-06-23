#pragma once
//#include <Core.h>
#include <Containers/DarkString.h>
#include <ctime>
#include <list>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include "optick.h"

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include "../CommandLine/CommandLine.h"
#include "Math/MathFwd.h"
#include "Containers/Array.h"



//#define Log(...) Logger::logF(__VA_ARGS__, nullptr)





struct log_
{
	size_t severenty = 0;
	FString txt = TEXT("");
	time_t time = time_t();
	bool isConsole = false;

	///////
	FString Result = TEXT("");
	int32 Line = 0;
	FString File = TEXT("");
	bool ModernLog = false;

	FVector Color = FVector();
};



enum LOGGER_ENUM
{
	LOGGER_INFO =	 0b00000001,
	LOGGER_WARNING = 0b00000010,
	LOGGER_ERROR =	 0b00000100,
	LOGGER_DEBUG =	 0b00001000,
	LOGGER_CONSOLE = 0b00010000
} typedef;



class Logger;

void logging(Logger*);
void record(log_&);

class DENGINE_API Logger
{
	friend void logging(Logger*);
	friend void record(log_&);
public:
	Logger();

	static void Initialize(size_t s);
	static void Exit();

	static void log(FString, LOGGER_ENUM severenty = LOGGER_INFO);
	static void logF(const char* arg, ...);
	static void logF(FString* arg, ...);
	static void exLog(FString, LOGGER_ENUM);
	static void log(log_ InLog);
	void urgLog(FString, LOGGER_ENUM);
#ifdef _WIN32
	static void log(std::wstring str, LOGGER_ENUM severenty = LOGGER_INFO);
#endif // _WIN32
	static void wait();

	//static std::vector<log_>& GetLogs() { return inst->Logs; }


private:
	static void ClearLogs();

public:
	std::vector<log_> TempLogs;

private:
	size_t severity = 0;
	std::vector <log_> logs;
	bool isWork = true;
	const uint64 MAX_LOGS = 1024;
	mutable std::mutex Mutex;
	std::thread LogThread;
private:
	static Logger* inst;
};



