#pragma once
//#include "ScriptState.h"
#include "sol2/sol.hpp"


class FScriptManager
{
	friend class FScriptState;
public:
	FScriptManager();

	static FScriptManager* Get();





public:
//private:
	sol::state VMLua;

};