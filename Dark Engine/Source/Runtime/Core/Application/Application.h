#pragma once
#include <Core.h>
#include <Windows.h>
#include <Containers/DarkString.h>
#include <Containers/Array.h>
#include <algorithm>


struct FCMDParameter
{
	FString Parameter;
	FString Value;
};


class DENGINE_API Application
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
	/*FString GetParameter(FString param) { return std::find_if(parameters.begin(), parameters.end(),
		[&param](const FCMDParameter &v){ return v.Parameter == param;})->Value;}*/


public:
	static HINSTANCE GetInstance() { return _inst->hInsance; }
	static INT32 CmdShow() { return _inst->nCmdShow; }
	


private:
	HINSTANCE hInsance;
	HINSTANCE nPrevInstance;
	FString lpCmdLine;
	INT32 nCmdShow;

private:
	TArray<FCMDParameter> parameters;
	







};