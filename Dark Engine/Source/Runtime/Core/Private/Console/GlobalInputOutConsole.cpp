#include "Console/GlobalInputOutConsole.h"



FGlobalInputOutConsole GGlobalConsole;




void FGlobalInputOutConsole::Initialize(bool bCreateOsConsole)
{
	if (bCreateOsConsole)
	{
		FConsoleStd::CreateConsoleOS(TEXT("Dark Console"));
	}
	bInitialize = true;

	RegisterConsoleCommand(TEXT("Clear"), [this](const TArray<FString>&)
		{
			this->Cache.Empty();
		}, TEXT("Clear console"));
	RegisterConsoleCommand(TEXT("Help"), [this](const TArray<FString>& Args)
		{
			if (Args.Num() > 0)
			{
				for (const auto& i : Args)
				{
					auto It = this->GetConsoleObjects().find(i);

					if (It != this->GetConsoleObjects().end())
					{
						if (It->second->AsVariable())
						{
							AddLog(FString::PrintF(TEXT("Var: %s = %s ( %s )"), *It->first,
								*It->second->AsVariable()->ToString(), *It->second->Description));
						}
						else
						{
							AddLog(FString::PrintF(TEXT("Command: %s ( %s )"), *It->first, *It->second->Description));
						}
					}
					else
					{
						AddLog(FString::PrintF(TEXT("Not found: %s"), *i));
					}
				}

				return;
			}

			for (const auto& i : this->GetConsoleObjects())
			{
				if (i.second->AsVariable())
				{
					AddLog(FString::PrintF(TEXT("Var: %s = %s ( %s )"), *i.first,
						*i.second->AsVariable()->ToString(), *i.second->Description));
				}
				else
				{
					AddLog(FString::PrintF(TEXT("Command: %s ( %s )"), *i.first, *i.second->Description));
				}
			}
		}, TEXT("Get all command, and get description"));

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
