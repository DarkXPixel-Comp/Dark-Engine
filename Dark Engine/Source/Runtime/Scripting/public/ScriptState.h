#pragma once

#include "Containers/DarkString.h"
#include "Memory/TUniquePtr.h"
#include "ScriptManager.h"
#include "sol2/sol.hpp"


class FScriptState
{
	friend class TScriptState;
private:
	//FScriptState();
public:
	template<typename RetValue, typename ClassValue, typename... Params>
	void AddFunction(const FString& NameFunc, RetValue(ClassValue::* Func)(Params...))
	{
		try
		{
			sol::table StateClass = FScriptManager::Get()->VMLua[-NameState];
			StateClass.set_function(-NameFunc, [Func](ClassValue& Obj, Params... InParams)
				{
					(Obj.*Func)(InParams...);
				});
		}
		catch (...)
		{
		}
	}

	template<typename... Params>
	void CallFunction(const FString& NameFunc, Params... InParams)
	{
		try
		{
			sol::optional<sol::function> MaybeFunc = FScriptManager::Get()->VMLua[-NameState]["BeginPlay"];
			if (MaybeFunc.has_value())
			{
				MaybeFunc->call(InParams...);
			}
		}
		catch(...)									 
		{
		}
	}



private:
	FString NameState;
};

class TScriptState
{
public:
	template<typename T>
	static TSharedPtr<class FScriptState> GetAndCreateScriptState()
	{
		TSharedPtr<class FScriptState> Result = MakeShareble(new FScriptState());
		FScriptManager* Manager = FScriptManager::Get();
		auto UserType = Manager->VMLua.new_usertype<T>(-T::StaticClass()->GetName());
		//Manager->VMLua[-InClass->GetName()] = Manager->VMLua.create_table();
		Result->NameState = T::StaticClass()->GetName();
		return Result;
	}

};

