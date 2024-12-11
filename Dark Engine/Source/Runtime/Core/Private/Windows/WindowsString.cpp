#include "Windows/WindowsString.h"
#include "Containers/DarkString.h"
#include "Containers/Array.h"
#include "Windows/WindowsIncludes.h"

void FWindowsString::MultiByteToWideChar(const ANSICHAR* InStr, FString& OutStr)
{
	int32 Len = (int32)strlen(InStr);
	int32 sz = ::MultiByteToWideChar(CP_UTF8, 0, InStr, Len, nullptr, 0);
	if (sz < 1)
	{
		OutStr = TEXT("");
	}
	else
	{
		std::wstring Result(sz, '\0');
		::MultiByteToWideChar(CP_UTF8, 0, InStr, Len, Result.data(), sz);
		OutStr = Result;
	}
}
