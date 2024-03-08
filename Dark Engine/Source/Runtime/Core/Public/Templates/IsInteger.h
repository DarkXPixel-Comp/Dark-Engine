#pragma once
#include "CoreTypes.h"

template <typename T>
struct TIsInteger
{
	enum { Value = false };
};

template <> struct TIsInteger<int8> { enum { Value = true }; };
template <> struct TIsInteger<uint8> { enum { Value = true }; };
template <> struct TIsInteger<int16> { enum { Value = true }; };
template <> struct TIsInteger<uint16> { enum { Value = true }; };
template <> struct TIsInteger<int32> { enum { Value = true }; };
template <> struct TIsInteger<uint32> { enum { Value = true }; };
template <> struct TIsInteger<int64> { enum { Value = true }; };
template <> struct TIsInteger<uint64> { enum { Value = true }; };

template <typename T> struct TIsInteger<const			T> { enum { Value = TIsFloatingPoint<T>::Value }; };
template <typename T> struct TIsInteger<volatile		T> { enum { Value = TIsFloatingPoint<T>::Value }; };
template <typename T> struct TIsInteger<const volatile T> { enum { Value = TIsFloatingPoint<T>::Value }; };
