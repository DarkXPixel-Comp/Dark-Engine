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
	OnAddConsoleInput.BroadCast(Log);
}