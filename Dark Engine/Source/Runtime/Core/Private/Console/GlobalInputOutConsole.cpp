#include "Console/GlobalInputOutConsole.h"



FGlobalInputOutConsole GGlobalConsole;

void FGlobalInputOutConsole::Initialize(bool bCreateOsConsole)
{
	if (bCreateOsConsole)
	{
		FConsoleStd::CreateConsoleOS(TEXT("Dark Console"));
	}
	bInitialize = true;
}

void FGlobalInputOutConsole::Destroy()
{
	bInitialize = false;
}

void FGlobalInputOutConsole::AddLog(const FString& Text, FVector3f Color, float Time)
{
	if (bInitialize)
	{
		FConsoleStd::AddLog(Text, Color, Time);
	}
}
