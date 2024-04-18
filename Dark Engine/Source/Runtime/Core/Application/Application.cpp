#include "Application.h"
#include <CommandLine/CommandLine.h>

Application* Application::_inst;

Application app;

Application::Application()
{
	_inst = this;
}

Application::~Application()
{
}

void Application::Initialize(HINSTANCE inst, HINSTANCE prevInst, FString cmdLine, INT32 cmdshow)
{
	hInsance = inst;
	nPrevInstance = prevInst;
	lpCmdLine = cmdLine;
	nCmdShow = cmdshow;

	ParseCmdLine();
}

void Application::ParseCmdLine()
{

}

bool Application::CheckParameterCmdLine(FString Parametr, FString& data)
{
	return false;
}
