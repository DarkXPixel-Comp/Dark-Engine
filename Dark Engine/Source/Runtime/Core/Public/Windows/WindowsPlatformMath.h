#pragma once


#include <Definitions.h>



#if !PLATFORM_WINDOWS
#error this code should only be included on Windows
#endif

#include <Microsoft/MicrosoftPlatformMath.h>

typedef FMicrosoftPlatformMathBase FWindowsPlatformMath;
typedef FWindowsPlatformMath FPlatformMath;