#pragma once
#include "Console/BaseConsole.h"




class FConsoleStd : public FBaseConsole
{
public:
	FConsoleStd() = default;
	~FConsoleStd();
	

	void CreateConsoleOS(const FString& Label);
	void DestroyConsole();
private:
	bool bInitializeConsole = false;


};

