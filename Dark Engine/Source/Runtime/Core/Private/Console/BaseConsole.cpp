#include "Console/BaseConsole.h"
#include <sstream>

uint64 FBaseConsole::StaticID = 0;





class FConsoleCommand : public IConsoleCommand
{
public:
	FConsoleCommand(const FConsoleCommandWithArgsDelegate& InitDelegate):
		Delegate(InitDelegate)
	{

	}
	FConsoleCommand(FConsoleSoloCommandWithArgsDelegate InitDelegate)
	{
		Delegate.Bind(InitDelegate);
	}



	virtual class IConsoleCommand* AsCommand() override
	{
		return this;
	}

	virtual bool Execute(const TArray<FString>& Args) override
	{
		Delegate.BroadCast(Args);
		return true;
	}
	
private:
	FConsoleCommandWithArgsDelegate Delegate;
};




IConsoleObject* FBaseConsole::AddConsoleObject(const TCHAR* Name, IConsoleObject* Obj)
{
	check(Name);
	check(Obj);

	auto It = ConsoleObjects.find(Name);

	IConsoleObject* Object = It != ConsoleObjects.end() ? It->second : nullptr;

	if (Object)
	{
		return Object;
	}
	else
	{
		ConsoleObjects.emplace(Name, Obj);
		return Obj;
	}

}


IConsoleCommand* FBaseConsole::RegisterConsoleCommand(const TCHAR* Name, const FConsoleCommandWithArgsDelegate& Command)
{
	return (IConsoleCommand*)AddConsoleObject(Name, new FConsoleCommand(Command));
}

IConsoleCommand* FBaseConsole::RegisterConsoleCommand(const TCHAR* Name, FConsoleSoloCommandWithArgsDelegate Command)
{
	return (IConsoleCommand*)AddConsoleObject(Name, new FConsoleCommand(Command));
}


void FBaseConsole::ParseInput(FString& InputOutCommands, TArray<FString>& OutArgs)
{
	FString InputCommand;
	std::wstringstream Stream(InputOutCommands.GetNativeString());
	std::getline(Stream, InputCommand.GetNativeString(), TEXT(' '));

	while (Stream.good())
	{
		FString Arg;
		std::getline(Stream, Arg.GetNativeString(), TEXT(' '));
		OutArgs.Add(Arg);
	}
	InputOutCommands = InputCommand;
}


FBaseConsole::~FBaseConsole()
{
	for (auto& i : ConsoleObjects)
	{
		delete i.second;
	}
}

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
	Inputs.Add(Text);
	OnAddConsoleInput.BroadCast(Text);
	TArray<FString> Args;
	FString Command = Text;

	ParseInput(Command, Args);

	auto It = ConsoleObjects.find(Command);

	if (It != ConsoleObjects.end())
	{
		if (It->second->AsCommand())
		{
			It->second->AsCommand()->Execute(Args);
		}
	}
	else
	{
		AddLog(Text, FVector3f(1, 1, 1));
	}
}
