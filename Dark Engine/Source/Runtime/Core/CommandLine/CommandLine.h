#pragma once

#include <Windows.h>
#include <string>

#define PrintLine(...) CommandConsole::PrintF(__VA_ARGS__, nullptr)

using namespace std;

class CommandConsole
{
	static CommandConsole* _inst;

	HANDLE hStdOut;

	bool onWork = false;

public:
	CommandConsole();

	static void Initialize(const char* consoleTitle);

	static void Print(const char* text);

	static void PrintF(const char* arg, ...);

	static void PrintF(string arg, ...);

	static bool isWork();

	void Clear();

};
