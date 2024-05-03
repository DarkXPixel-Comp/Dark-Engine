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


template<class T>
class FConsoleVariableRef : public IConsoleVariable
{
public:
	FConsoleVariableRef(T& InRefValue):
		RefValue(InRefValue)
	{}

	virtual class IConsoleVariable* AsVariable()
	{
		return this;
	}


	virtual void Set(const TCHAR* InValue)
	{
		FString::FromString(RefValue, InValue);
		OnChangedCallback.BroadCast(this);
	}

	virtual void SetOnChangedCallback(const FConsoleVariableDelegate& Callback) override
	{
		OnChangedCallback = Callback;
	}
	virtual void SetOnChangedCallback(FConsoleSoloVariableDelegate Callback) override
	{
		OnChangedCallback.Bind(Callback);
	}


	virtual bool GetBool() const override { return false; }
	virtual int32 GetInt() const { return 0; }
	virtual float GetFloat() const { return 0; }
	virtual FString GetString() const { return FString(); }

	virtual FString ToString() const override
	{
		return FString::NumToString(RefValue);
	}


	virtual bool IsVariableBool() const override { return false; }
	virtual bool IsVariableInt() const override { return false; }
	virtual bool IsVariableFloat() const override { return false; }
	virtual bool IsVariableString() const override { return false; }


private:
	T& RefValue;
	FConsoleVariableDelegate OnChangedCallback;
};

template<>
bool FConsoleVariableRef<bool>::GetBool() const
{
	return RefValue;
}
template<>
int32 FConsoleVariableRef<int32>::GetInt() const
{
	return RefValue;
}
template<>
float FConsoleVariableRef<float>::GetFloat() const
{
	return RefValue;
}
template<>
FString FConsoleVariableRef<FString>::GetString() const
{
	return RefValue;
}



template<>
bool FConsoleVariableRef<bool>::IsVariableBool() const
{
	return true;
}
template<>
bool FConsoleVariableRef<int32>::IsVariableInt() const
{
	return true;
}
template<>
bool FConsoleVariableRef<float>::IsVariableFloat() const
{
	return true;
}
template<>
bool FConsoleVariableRef<FString>::IsVariableString() const
{
	return true;
}


IConsoleObject* FBaseConsole::AddConsoleObject(const TCHAR* Name, IConsoleObject* Obj, const TCHAR* InDescription)
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
		Obj->Description = InDescription;
		ConsoleObjects.emplace(Name, Obj);
		return Obj;
	}

}


IConsoleCommand* FBaseConsole::RegisterConsoleCommand(const TCHAR* Name, const FConsoleCommandWithArgsDelegate& Command, const TCHAR* Description)
{
	return (IConsoleCommand*)AddConsoleObject(Name, new FConsoleCommand(Command), Description);
}

IConsoleCommand* FBaseConsole::RegisterConsoleCommand(const TCHAR* Name, FConsoleSoloCommandWithArgsDelegate Command, const TCHAR* Description)
{
	return (IConsoleCommand*)AddConsoleObject(Name, new FConsoleCommand(Command), Description);
}

IConsoleVariable* FBaseConsole::RegisterConsoleVariableRef(const TCHAR* Name, int32& RefValue, const TCHAR* Description)
{
	return (IConsoleVariable*)AddConsoleObject(Name, new FConsoleVariableRef<int32>(RefValue), Description);
}

void FBaseConsole::ParseInput(FString& InputOutCommands, TArray<FString>& OutArgs)
{
	FString InputCommand = TEXT("\0");
	InputCommand.Resize(256);
	std::wstringstream Stream(InputOutCommands.GetNativeString());
	std::getline(Stream, InputCommand.GetNativeString(), TEXT(' '));

	while (Stream.good())
	{
		FString Arg;
		std::getline(Stream, Arg.GetNativeString(), TEXT(','));
		while (Arg.Back() == TEXT(' '))
		{
			Arg.PopBack();
		}
		while (Arg.First() == TEXT(' '))
		{
			Arg.PopFirst();
		}
		/*if (Arg.Back() == TEXT(' '))
		{
			Arg.PopBack();
		}
		if (Arg.First() == TEXT(' '))
		{
			Arg.PopFirst();
		}*/
		if (Arg == TEXT(""))
			continue;
		OutArgs.Add(Arg);
	}
	InputOutCommands = InputCommand;
}


FBaseConsole::FBaseConsole()
{
	bInit = true;
}

FBaseConsole::~FBaseConsole()
{
	for (auto& i : ConsoleObjects)
	{
		delete i.second;
	}
	ConsoleObjects.clear();
	bInit = false;
}

void FBaseConsole::AddLog(const FString& Text, FVector3f Color, float Time)
{
	if (!bInit)
		return;
	FScopeLock Lock(&AddLogCriticalSection);
	FConsoleLog Log;
	Log.Log = Text;
	Log.Id = StaticID++;
	Log.Color = Color;
	Log.Time = Time;
	Cache.Add(Log);
	OnAddConsoleMessage.BroadCast(Log);
}

TArray<FString> FBaseConsole::InputText(const FString& Text, bool Check)
{
	if (!bInit)
		return TArray<FString>();

	TArray<std::pair<FString, IConsoleObject*>>	FindObjects;
	TArray<FString>	Out;
	TArray<FString> Args;

	Inputs.Add(Text);
	OnAddConsoleInput.BroadCast(Text);
	FString Command = Text;

	ParseInput(Command, Args);

	std::copy_if(ConsoleObjects.begin(), ConsoleObjects.end(), std::back_inserter(FindObjects.GetVector()),
		[&Command](const std::pair<FString, IConsoleObject*>& Obj)
		{
			return Obj.first.MinStartsWithMin(Command);
		});

	if (FindObjects.Num() == 1 && !Check)
	{
		auto It = FindObjects[0];
		Command = It.first;
		if (It.second->AsCommand())
		{
			It.second->AsCommand()->Execute(Args);
		}
		if (It.second->AsVariable())
		{
			if (Args.Num() > 0)
			{
				It.second->AsVariable()->Set(Args[0]);
				//AddLog(Command, FVector3f(1, 1, 1));
				AddLog(FString::PrintF(TEXT("%s %s"), *Command, *Args[0]));
			}
			else
			{
				AddLog(FString::PrintF(TEXT("%s %s"), *Command, *It.second->AsVariable()->ToString()));
			}
		}
	}

	std::transform(FindObjects.begin(), FindObjects.end(), std::back_inserter(Out.GetVector()), [](const auto& Obj)
		{
			return Obj.first;
		});
	return Out;
}
