#pragma once
#include "Console/ConsoleStd.h"

extern class FGlobalInputOutConsole GGlobalConsole;


class FGlobalInputOutConsole : public FConsoleStd
{
public:
	FGlobalInputOutConsole() = default;
	~FGlobalInputOutConsole()
	{
		Destroy();
	}
	void Initialize(bool bCreateOsConsole = false);
	void Destroy();

	virtual void AddLog(const FString& Text, FVector3f Color = FVector3f(1, 1, 1), float Time = 0.f) override;
private:
	bool bInitialize = false;

};