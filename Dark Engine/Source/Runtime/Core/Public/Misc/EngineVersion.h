#pragma once
#include "Platform/Platform.h"


CORE_API int32 GetMajorVersion();
CORE_API int32 GetMinorVersion();
CORE_API int32 GetPatchVersion();
CORE_API int32 GetFixVersion();

CORE_API const TCHAR* GetStringVersionT();
CORE_API const TCHAR* GetStringBuildTypeT();
CORE_API int32 GetNumberVersion();