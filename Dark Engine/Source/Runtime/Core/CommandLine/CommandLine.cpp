#include "CommandLine.h"

CommandConsole* CommandConsole::_inst = nullptr;

CommandConsole console;



CommandConsole::CommandConsole()
{
	if (_inst)
		return;

	_inst = this;
}


void CommandConsole::Initialize(const char* consoleTitle = "Debug Engine console")
{
#if _WIN64
	AllocConsole();

	AttachConsole(GetCurrentProcessId());

	_inst->hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	_inst->onWork = true;


	SetConsoleTitleA(consoleTitle);

#elif
#error This OS not support
#endif // _WIN64
}


void CommandConsole::Print(const char* text)
{
#ifdef _WIN64
	DWORD dwCount = 0;

	for (;text[dwCount] != '\0'; ++dwCount){}
	
	WriteConsoleA(_inst->hStdOut, text, dwCount, NULL, NULL);
#elif
#error This OS not Support
#endif // _WIN64
}


bool CommandConsole::isWork()
{
	return _inst->onWork;
}