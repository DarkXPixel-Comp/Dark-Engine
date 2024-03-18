#pragma once

#include "HAL/Platform.h"

#if !PLATFORM_WINDOWS
#include "Microsoft/PreWindowsApi.h"
#else
#if !defined(__clang__)
#pragma warning(disable:4103)
#endif

#pragma push_macro("MAX_uint8")
#pragma push_macro("MAX_uint16")
#pragma push_macro("MAX_uint32")
#pragma push_macro("MAX_uint64")
#pragma push_macro("TEXT")
#pragma push_macro("TRUE")
#pragma push_macro("FALSE")


#ifndef _WINNT_
#undef TEXT

#endif

#endif




