#pragma once

#include <Core.h>
#include <Windows.h>
#include <Containers/DarkString.h>

#define PrintLine(...) CommandConsole::Prints(__VA_ARGS__, nullptr)

using namespace std;

class DENGINE_API CommandConsole
{
	static CommandConsole* _inst;

	HANDLE hStdOut;

	bool onWork = false;

public:
	CommandConsole();

	static void Initialize(const char* consoleTitle);

	static void Print(const char* text);

	static void Prints(const char* arg, ...);

	static void Prints(string arg, ...);

	static bool isWork();

	void Clear();

};
