#include "GenericPlatform/GenericPlatformMisc.h"

void FGenericPlatformMisc::LocalPrint(const TCHAR* Str)
{
#ifdef UNICODE
	wprintf(TEXT("%s"), Str);
#else
	printf("%s", Str);
#endif
}
