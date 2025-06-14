#include "Misc/EngineVersion.h"
#include "Containers/DarkString.h"


const int32 MAJOR_VERSION = 0;
const int32 MINOR_VERSION = 2;
const int32 PATCH_VERSION = 18;
const int32 FIX_VERSION = 0;
const TCHAR* VERSION_PREFIX = TEXT("NewLife");
const TCHAR* BUILD_TYPE_PREFIX = TEXT("Debug");
const constexpr int32 NUMBER_VERSION = MAJOR_VERSION * 1000 + MINOR_VERSION * 100 + PATCH_VERSION * 10 + FIX_VERSION * 1;
FString StringVersion;

int32 GetMajorVersion()
{
	return MAJOR_VERSION;
}

int32 GetMinorVersion()
{
	return MINOR_VERSION;
}

int32 GetPatchVersion()
{
	return PATCH_VERSION;
}

int32 GetFixVersion()
{
	return 0;
}

const TCHAR* GetStringVersionT()
{
	if (StringVersion.Lenght())
		return *StringVersion;
	
	StringVersion = FString::PrintF(TEXT("v%i.%i.%i.%i-%s"), MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, FIX_VERSION, VERSION_PREFIX);
	return *StringVersion;
}

const TCHAR* GetStringBuildTypeT()
{
	return BUILD_TYPE_PREFIX;
}

int32 GetNumberVersion()
{
	return NUMBER_VERSION;
}
