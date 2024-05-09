#pragma once
#include <stdlib.h>
#include "Misc/AssertionMacros.h"
#include <iterator>


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

template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T&> { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };





template <typename T>
typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;
	return (CastType&&)Obj;
}



class FNoncopyble
{
protected:
	FNoncopyble() {}
	~FNoncopyble() {}
private:
	FNoncopyble(const FNoncopyble&);
	FNoncopyble& operator=(const FNoncopyble&);
};



template <typename T>
FORCEINLINE T BitMask(uint32 Count);

template <>
FORCEINLINE uint64 BitMask<uint64>(uint32 Count)
{
	return (uint64(Count < 64) << Count) - 1;
}

template <>
FORCEINLINE uint32 BitMask<uint32>(uint32 Count)
{
	return uint32(uint64(1) << Count) - 1;
}

template <>
FORCEINLINE uint16 BitMask<uint16>(uint32 Count)
{
	return uint16((uint32(1) << Count) - 1);
}

template <>
FORCEINLINE uint8 BitMask<uint8>(uint32 Count)
{
	return uint8((uint32(1) << Count) - 1);
}

