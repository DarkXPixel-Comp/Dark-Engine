// Copyright DarkXPixel, Inc. All Rights Reserved.

#pragma once


/*---------------------------------------------
		Low Level Includes
---------------------------------------------*/

#include "CoreTypes.h"
#include "CorePlatform.h"



/*---------------------------------------------
		Forward
---------------------------------------------*/

#include <string>

#include "ConfigManager/FConfigManager.h"

#include "Settings/PerEngineSettings.h"

#include "Logging/Logger.hpp"

#include "CommandLine/CommandLine.h"

#define istr(x) std::to_string(x)
#define icstr(x) std::to_string(x).c_str()
#define cstr(x) x.c_str()
#define strw(x) std::wstring(x.begin(), x.end())
#define wstr(x) std::string(x.begin(), x.end())

//#include <Source/Runtime/Engine/public/DEngine.h>

