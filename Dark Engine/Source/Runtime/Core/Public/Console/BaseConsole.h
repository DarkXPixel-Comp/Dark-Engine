#pragma once
#include "Misc/Delegate.h"
#include "Containers/DarkString.h"


DECLARE_DELEGATE_OneParam(FOnAddConsoleInput, FString);
DECLARE_DELEGATE_OneParam(FOnAddConsoleMessage, struct FConsoleLog);


struct FConsoleLog
{
	uint64 Id;
	FString Log;
	//FVector3f Color;
	float Time;
};

template<class T>
class TConsoleVariableData
{
public:
	TConsoleVariableData(const T DefaultValue) :
		Data(DefaultValue)
	{
	}

	T GetValue() const
	{
		return Data;
	}

	void SetValue(const T& Value)
	{
		Data = Value;
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