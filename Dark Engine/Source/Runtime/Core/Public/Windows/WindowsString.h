#pragma once
#include "GenericPlatform/GenericString.h"



struct FWindowsString : public FGenericString
{
	static void CORE_API MultiByteToWideChar(const ANSICHAR* InStr, class FString& OutStr);
};

typedef FWindowsString FPlatformString;