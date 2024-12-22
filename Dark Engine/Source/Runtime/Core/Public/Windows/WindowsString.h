#pragma once
#include "GenericPlatform/GenericString.h"
#include <string>



struct FWindowsString : public FGenericString
{
	[[deprecated]] static void CORE_API MultiByteToWideChar(const ANSICHAR* InStr, class FString& OutStr);

	static void CORE_API MultiByteToWideChar(const ANSICHAR* InStr, std::wstring& OutStr);
	static void CORE_API WideCharToMultiByte(const WIDECHAR* InStr, std::string& OutStr);
};

typedef FWindowsString FPlatformString;