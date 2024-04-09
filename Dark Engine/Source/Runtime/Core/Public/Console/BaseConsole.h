#pragma once
#include "Containers/DarkString.h"
#include "Containers/Array.h"
#include "Delegate/Delegate.h"


DECLARE_DELEGATE_OneParam(FOnAddConsoleInput, FString);
DECLARE_DELEGATE_OneParam(FOnAddConsoleMessage, struct FConsoleLog);



struct FConsoleLog
{
	uint64 Id;
	FString Log;
	FVector3f Color;
	float Time;

};


class FBaseConsole
{
public:
	FBaseConsole() = default;

	virtual void AddLog(const FString& Text, FVector3f Color = FVector3f(0, 0, 0), float Time = 0.f);
	void Update(float DeltaTime);
	void RemoveLog();
	void InputText(const FString& Text);

	const TArray<FConsoleLog>& GetLogs() const { return Cache; }

	FOnAddConsoleInput OnAddConsoleInput;  
	FOnAddConsoleMessage OnAddConsoleMessage;
protected:
	TArray<FConsoleLog> Cache;
	TArray<FString>	Inputs;
	static uint64 StaticID;


};