#pragma once


#include <CoreTypes.h>
#include <Templates/EnableIf.h>
#include <type_traits>
#include "IsFloatingPoint.h"

#if __cplusplus < 202000
	#define	DE_REQUIRES(...) , std::enable_if_t<(__VA_ARGS__), int>= 0
#else
//namespace DE::Core::Private
//{
//	template<bool B>
//	concept BoolIdentityConcept = B;
//}
//#define UE_DE_REQUIRES > requires (!!(__VA_ARGS__))	 && DE::Core::Private::BoolIdentityConcept<true
#endif

#define TEMPLATE_REQUIRES(...) typename TEnableIf<__VA_ARGS__, int>::type = 0