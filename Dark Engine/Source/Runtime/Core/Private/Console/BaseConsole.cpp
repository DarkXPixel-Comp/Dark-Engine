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

IConsoleVariable* FBaseConsole::RegisterConsoleVariableRef(const TCHAR* Name, int32& RefValue)
{
	return (IConsoleVariable*)AddConsoleObject(Name, new FConsoleVariableRef<int32>(RefValue));
}

void FBaseConsole::ParseInput(FString& InputOutCommands, TArray<FString>& OutArgs)
{
	FString InputCommand = TEXT("\0");
	InputCommand.Resize(256);
	std::wstringstream Stream(InputOutCommands.GetNativeString());
	std::getline(Stream, InputCommand.GetNativeString(), TEXT(' '));

	/*Stream.getline(InputCommand.Data(), 256, TEXT(' '));

	TCHAR Buffer[256] = TEXT("\0");
	bool ArgOpen = false;
	while (Stream.good())
	{
		wchar_t buf;
		Stream.get(buf);
		if (buf == TEXT('"') || buf == TEXT(' '))
		{
			if (ArgOpen)
			{
				ArgOpen = false;	
				if(FString(Buffer) != TEXT(""))
					OutArgs.Add(Buffer);
				wcsset(Buffer, TEXT('\0'));
			}
			else
			{
				ArgOpen = true;
			}
			continue;
		}
		if(ArgOpen)
			wcscat(Buffer, &buf);
	}*/


	while (Stream.good())
	{
		FString Arg;
		std::getline(Stream, Arg.GetNativeString(), TEXT(','));
		if (Arg.Back() == TEXT(' '))
		{
			Arg.PopBack();
		}
		if (Arg.First() == TEXT(' '))
		{
			Arg.PopFirst();
		}
		if (Arg == TEXT(""))
			continue;
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
	//AddLog(Text, FVector3f(1, 1, 1));
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
		if (It->second->AsVariable())
		{
			if (Args.Num() > 0)
			{
				It->second->AsVariable()->Set(Args[0]);
				AddLog(Text, FVector3f(1, 1, 1));
			}
			else
			{
				AddLog(FString::PrintF(TEXT("%s %s"), *Command, *It->second->AsVariable()->ToString()));
			}
		}
	}
	else
	{
		//AddLog(Text, FVector3f(1, 1, 1));
	}
}
