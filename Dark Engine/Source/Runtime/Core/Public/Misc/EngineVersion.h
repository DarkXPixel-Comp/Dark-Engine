#pragma once
#include "HAL/Platform.h"


extern int32 GetMajorVersion();
extern int32 GetMinorVersion();
extern int32 GetPatchVersion();
extern int32 GetFixVersion();

extern const TCHAR* GetStringVersionT();
extern const ANSICHAR* GetStringVersionA();
extern const TCHAR* GetStringBuildTypeT();
extern int32 GetNumberVersion();