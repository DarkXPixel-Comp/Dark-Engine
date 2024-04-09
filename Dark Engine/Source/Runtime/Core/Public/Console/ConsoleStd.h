#pragma once
#include "Console/BaseConsole.h"




class FConsoleStd : public FBaseConsole
{
public:
	FConsoleStd() = default;
	~FConsoleStd();
	

	virtual void AddLog(const FString& Text, FVector3f Color = FVector3f(0, 0, 0), float Time = 0.f) override;
	void CreateConsoleOS(const FString& Label);
	void DestroyConsole();
	void SetAutoClose(bool InbAutoClose);
private:
	bool bInitializeConsole = false;
	bool bAutoClose = true;


};

