#pragma once
#include <cstdarg>
#include "Platform/PlatformString.h"

#define GET_VARARGS_RESULT(msg, msgsize, len, lastarg, fmt, result)\
{			  \
	va_list ap;		\
	va_start(ap, lastarg); \
	result = FPlatformString::StringPrintf(msg, msgsize, fmt, ap);	 \
	if (result >= msgsize)		  \
	{				  \
		result = -1;	   \
	}					\
	va_end(ap);		   \
}				