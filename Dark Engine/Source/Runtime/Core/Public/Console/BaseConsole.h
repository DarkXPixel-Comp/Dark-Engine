#pragma once
#include "Containers/DarkString.h"
#include "Containers/Array.h"
#include "Containers/UnordoredMap.h"
#include "Delegate/Delegate.h"


DECLARE_DELEGATE_OneParam(FOnAddConsoleInput, FString);
DECLARE_DELEGATE_OneParam(FOnAddConsoleMessage, struct FConsoleLog);


typedef std::function<void(const TArray<FString>&)>	FConsoleSoloCommandWithArgsDelegate;
typedef std::function<void(class IConsoleVariable*)> FConsoleSoloVariableDelegate;



typedef TMultiCastDelegate<void, class IConsoleVariable*> FConsoleVariableDelegate;
typedef TMultiCastDelegate<void, const TArray<FString>&> FConsoleCommandWithArgsDelegate;


struct FConsoleLog
{
	uint64 Id;
	FString Log;
	FVector3f Color;
	float Time;
};


template<class T>
class TConsoleVariableData
{
public:
	TConsoleVariableData(const T DefaultValue):
		Data(DefaultValue)
	{
	}

	T GetValue() const
	{
		return Data;
	}


private:
	T Data;

};


class IConsoleObject
{
public:
	virtual class IConsoleVariable* AsVariable()
	{
		return 0;
	}

	virtual bool IsVariableBool() const { return false; }
	virtual bool IsVariableInt() const { return false; }
	virtual bool IsVariableFloat() const { return false; }
	virtual bool IsVariableString() const { return false; }

	virtual TConsoleVariableData<bool>* AsVariableBool()
	{
		return 0;
	}
	virtual TConsoleVariableData<int32>* AsVariableInt()
	{
		return 0;
	}
	virtual TConsoleVariableData<float>* AsVariableFloat()
	{
		return 0;
	}
	virtual TConsoleVariableData<FString>* AsVariableString()
	{
		return 0;
	}

	virtual class IConsoleCommand* AsCommand()
	{
		return 0;
	}

	FString Description;
};

class IConsoleVariable : public IConsoleObject
{
public:
	virtual void Set(const TCHAR* InValue) = 0;
	virtual bool GetBool() const = 0;
	virtual int32 GetInt() const = 0;
	virtual float GetFloat() const = 0;
	virtual FString GetString() const = 0;

	void GetValue(int32& OutValue)
	{
		OutValue = GetInt();
	}
	void GetValue(bool& OutValue)
	{
		OutValue = GetBool();
	}
	void GetValue(float& OutValue)
	{
		OutValue = GetFloat();
	}
	void GetValue(FString& OutValue)
	{
		OutValue = GetString();
	}

	virtual FString ToString() const = 0;

	virtual void SetOnChangedCallback(const FConsoleVariableDelegate& Callback) = 0;
	virtual void SetOnChangedCallback(FConsoleSoloVariableDelegate Callback) = 0;

	void Set(bool InValue)
	{
		Set(InValue ? TEXT("1") : TEXT("0"));
	}
	void Set(int32 InValue)
	{
		Set(*FString::PrintF(TEXT("%i"), InValue));
	}
	void Set(float InValue)
	{
		Set(*FString::PrintF(TEXT("%f"), InValue));
	}
	void Set(const FString& InValue)
	{
		Set(*InValue);
	}
};

class IConsoleCommand : public IConsoleObject
{
public:
	virtual bool Execute(const TArray<FString>& Args) = 0;
};



class FBaseConsole
{
public:
	FBaseConsole() = default; 
	~FBaseConsole();

	virtual void AddLog(const FString& Text, FVector3f Color = FVector3f(1, 1, 1), float Time = 0.f);
	void Update(float DeltaTime);
	void RemoveLog();
	void InputText(const FString& Text);


	IConsoleObject* AddConsoleObject(const TCHAR* Name, IConsoleObject* Obj, const TCHAR* Description = TEXT(""));

	class IConsoleCommand* RegisterConsoleCommand(const TCHAR* Name, const FConsoleCommandWithArgsDelegate& Command, const TCHAR* Description = TEXT(""));
	class IConsoleCommand* RegisterConsoleCommand(const TCHAR* Name, FConsoleSoloCommandWithArgsDelegate Command, const TCHAR* Description = TEXT(""));


	class IConsoleVariable* RegisterConsoleVariable(const TCHAR* Name, const FString& DefaultValue, const TCHAR* Description = TEXT(""));
	class IConsoleVariable* RegisterConsoleVariable(const TCHAR* Name, float DefaultValue, const TCHAR* Description = TEXT(""));
	class IConsoleVariable* RegisterConsoleVariable(const TCHAR* Name, int32 DefaultValue, const TCHAR* Description = TEXT(""));
	class IConsoleVariable* RegisterConsoleVariable(const TCHAR* Name, bool DefaultValue, const TCHAR* Description = TEXT(""));

	class IConsoleVariable* RegisterConsoleVariableRef(const TCHAR* Name, FString& RefValue, const TCHAR* Description = TEXT(""));
	class IConsoleVariable* RegisterConsoleVariableRef(const TCHAR* Name, float& RefValue, const TCHAR* Description = TEXT(""));
	class IConsoleVariable* RegisterConsoleVariableRef(const TCHAR* Name, int32& RefValue, const TCHAR* Description = TEXT(""));
	class IConsoleVariable* RegisterConsoleVariableRef(const TCHAR* Name, bool& RefValue, const TCHAR* Description = TEXT(""));


	const TArray<FConsoleLog>& GetLogs() const { return Cache; }

	FOnAddConsoleInput OnAddConsoleInput;  
	FOnAddConsoleMessage OnAddConsoleMessage;

	const TUnordoredMap<FString, IConsoleObject*>& GetConsoleObjects() const
	{
		return ConsoleObjects;
	}


private:
	void ParseInput(FString& InputOutCommands, TArray<FString>& OutArgs);

protected:
	TArray<FConsoleLog> Cache;
	TArray<FString>	Inputs;
	static uint64 StaticID;

private:

	TUnordoredMap<FString, IConsoleObject*>	ConsoleObjects;

};