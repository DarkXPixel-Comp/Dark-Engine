#pragma once
#include "Platform/Platform.h"


#ifdef _DEBUG
#define check(expr)				DE_CHECK_IMPL(expr)

#define DE_CHECK_IMPL(expr)  \
	if(!expr) \
	{PLATFORM_BREAK();}

#else
#define check(expr)
#endif