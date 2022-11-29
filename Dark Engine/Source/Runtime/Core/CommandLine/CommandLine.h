#pragma once
#include "../CoreMinimal.h"


class CommandConsole
{
	static CommandConsole* _inst;

	HANDLE hStdOut;

	bool onWork = false;

public:
	CommandConsole();

	static void Initialize(const char* consoleTitle);

	static void Print(const char* text);

	static bool isWork();

	void Clear();

};
