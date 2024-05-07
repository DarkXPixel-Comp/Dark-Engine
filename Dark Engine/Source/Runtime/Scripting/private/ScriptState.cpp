#include "ScriptState.h"


FScriptState::FScriptState()
{
	State.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::utf8);
}

FScriptState::~FScriptState()
{
}
