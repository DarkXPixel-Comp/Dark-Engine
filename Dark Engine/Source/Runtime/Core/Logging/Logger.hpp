#pragma once
#include <Containers/String/DarkString.h>
#include <ctime>
#include <list>
#include <fstream>
#include <thread>
#include <mutex>
#include <Containers/String/DarkString.h>

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
	LOGGER_INFO =	 0b000001,
	LOGGER_WARNING = 0b000010,
	LOGGER_ERROR =	 0b000100,
	LOGGER_DEBUG =	 0b001000,
	LOGGER_CONSOLE = 0b010000
} typedef;



class Logger;

void logging(Logger*);
void record(log_&);

class Logger
{
	friend void logging(Logger*);
	friend void record(log_&);

	static Logger* inst;

public:
	Logger();

	static void Initialize(size_t s);


	static void log(FString, LOGGER_ENUM severenty = LOGGER_INFO);
	static void logF(const char* arg, ...);
#ifdef _WIN32
	static void log(std::wstring str, LOGGER_ENUM severenty = LOGGER_INFO);
#endif // _WIN32

	void urgLog(FString, LOGGER_ENUM);

	static void exLog(FString, LOGGER_ENUM);

	static void wait();



private:
	size_t severenty;

	std::list <log_> logs;

	

};



