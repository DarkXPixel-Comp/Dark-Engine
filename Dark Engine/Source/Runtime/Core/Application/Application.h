#pragma once
#include <Windows.h>
#include <Containers/String/DarkString.h>




class Application
{
private:
	static Application* _inst;

public:
	static Application* Instance() { return _inst; }

public:
	Application();
	~Application();

	void Initialize(HINSTANCE, HINSTANCE, FString, INT32);

public:
	void ParseCmdLine();
	bool CheckParameterCmdLine(FString Parametr, FString& data);


public:
	static HINSTANCE GetInstance() { return _inst->hInsance; }
	static INT32 CmdShow() { return _inst->nCmdShow; }
	


private:
	HINSTANCE hInsance;
	HINSTANCE nPrevInstance;
	FString lpCmdLine;
	INT32 nCmdShow;
	







};