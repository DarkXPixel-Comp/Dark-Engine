#include "ScriptManager.h"
#include "Memory/TUniquePtr.h"


TUniquePtr<FScriptManager> ScriptManager;



FScriptManager::FScriptManager()
{
	VMLua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
}

FScriptManager* FScriptManager::Get()
{
	if (ScriptManager)
	{
		return ScriptManager.get();
	}
	ScriptManager = MakeUnique(new FScriptManager());
	return ScriptManager.get();
}
