#include "Console/BaseConsole.h"

uint64 FBaseConsole::StaticID = 0;

void FBaseConsole::AddLog(const FString& Text, FVector3f Color, float Time)
{
	FConsoleLog Log;
	Log.Log = Text;
	Log.Id = StaticID++;
	Log.Color = Color;
	Log.Time = Time;
	Cache.Add(Log);
	OnAddConsoleMessage.BroadCast(Log);
}

void FBaseConsole::InputText(const FString& Text)
{
	AddLog(Text, FVector3f(1, 1, 1));
	Inputs.Add(Text);
	OnAddConsoleInput.BroadCast(Text);
}
