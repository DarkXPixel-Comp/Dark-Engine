#pragma once

#include <intrin.h>

#if _MSC_VER < 1920
#error "Compiler is expected to support if constexpr"
#endif

//#if defined(__cpp_fold_expressions)
//#define PLATFORM_COMPILER_HAS_FOLD_EXPRESSIONS 1
//#else
//#error "Compiler is expected to support fold expressions"
//#endif

#define PLATFORM_RETURN_ADDRESS()	        _ReturnAddress()
#define PLATFORM_RETURN_ADDRESS_POINTER()	_AddressOfReturnAddress()
#define PLATFORM_FUNC_NAME __FUNCSIG__
