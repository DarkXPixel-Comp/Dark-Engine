#pragma once
#include <stdlib.h>
#include "Platform/Platform.h"

template <class Type, size_t Size>
constexpr size_t CountOf(const Type(&)[Size]) noexcept {
	return Size;
}

template <class Container>
_NODISCARD constexpr auto CountOf(const Container& Cont) noexcept(noexcept(Cont.Num())) -> decltype(Cont.Num())
{
	return Cont.Num();
}

#define DE_ARRAY_COUNT(Array) CountOf(Array)
#define DE_OFFSETOF(Class, Property) ((uint64)&reinterpret_cast<const int8 volatile&>((((Class*)0)->Property)))