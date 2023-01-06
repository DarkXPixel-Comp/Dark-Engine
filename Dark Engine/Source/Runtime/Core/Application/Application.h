#pragma once
#include <string>
#include <Core/CorePlatform.h>



class Application
{
	typedef std::string STRING;
private:
	static Application* _inst;

public:
	static Application* Instance() { return _inst; }

public:
	Application();
	~Application();

	void Initialize(HINSTANCE, HINSTANCE, STRING, INT32);

public:
	void ParseCmdLine();
	bool CheckParameterCmdLine(STRING Parametr, STRING& data);


public:
	static HINSTANCE GetInstance() { return _inst->hInsance; }
	static INT32 CmdShow() { return _inst->nCmdShow; }
	


private:
	HINSTANCE hInsance;
	HINSTANCE nPrevInstance;
	STRING lpCmdLine;
	INT32 nCmdShow;
	







};