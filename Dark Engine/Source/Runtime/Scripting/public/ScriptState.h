#pragma once
#include "lua.hpp"
#include "ScriptFunction.h"
#include "Containers/UnordoredMap.h"
#include "Containers/DarkString.h"
#include "sol2/sol.hpp"


class FScriptState
{
public:
	FScriptState();
	~FScriptState();


private:
	sol::state State;
};