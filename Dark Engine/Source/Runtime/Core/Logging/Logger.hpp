#pragma once
#include <Core.h>
#include <Containers/DarkString.h>
#include <ctime>
#include <list>
#include <fstream>
#include <thread>
#include <mutex>
#include <Containers/DarkString.h>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include "../CommandLine/CommandLine.h"



#define Log(...) Logger::logF(__VA_ARGS__, nullptr)



struct log_
{
	size_t severenty;

	FString txt;

	time_t time;

	bool isConsole = false;
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
	void urgLog(FString, LOGGER_ENUM);
#ifdef _WIN32
	static void log(std::wstring str, LOGGER_ENUM severenty = LOGGER_INFO);
#endif // _WIN32
	static void wait();



private:
	size_t severity = 0;
	std::list <log_> logs;
	bool isWork = true;
private:
	static Logger* inst;
};



