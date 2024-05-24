#pragma once
#include "HAL/PreprocessorHelpers.h"
#include "CoreTypes.h"
#include "Object.h"
#include <type_traits>


template <typename From, typename To> struct TCopyQualifiersFromTo { typedef                To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<const          From, To> { typedef const          To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<      volatile From, To> { typedef       volatile To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<const volatile From, To> { typedef const volatile To Type; };


template <typename From, typename To>
struct TLosesQualifiersFromTo
{
	enum { Value = !std::is_same_v<typename TCopyQualifiersFromTo<From, To>::Type, To> };
};


template <typename T, bool bIsAUObject_IMPL = std::is_convertible_v<T*, const volatile GObject*>>
struct TIsIInterface
{
	enum { Value = false };
};

template <typename T>
struct TIsIInterface<T, false>
{
	template <typename U> static char(&Resolve(typename U::UClassType*))[(U::UClassType::StaticClassFlags & CLASS_Interface) ? 2 : 1];
	template <typename U> static char(&Resolve(...))[1];

	enum { Value = sizeof(Resolve<T>(0)) - 1 };
};

template<typename T>
struct TIsCastable
{
	// It's from-castable if it's an interface or a UObject-derived type
	enum { Value = TIsIInterface<T>::Value || std::is_convertible_v<T*, const volatile UObject*> };
};

template <typename To, typename From>
FORCEINLINE To* Cast(From* Src)
{
	static_assert(sizeof(From) > 0 && sizeof(To) > 0, "Attempting to cast between incomplete types");

	if (Src)
	{
		if constexpr (TIsIInterface<From>::Value)
		{
			return nullptr;
		}
	}

}

template<typename To, typename From>
To* CastChecked(From* Src)
{
	static_assert(sizeof(From) > 0 && sizeof(To) > 0, "Attempting to cast between incomplete types");

	if (Src)
	{
		To* Result = Cast<To>(Src);
	}
}



template<typename To, typename From>
FORCEINLINE To DynamicCast(From&& Arg)
{
	using FromValueType = std::remove_reference_t<From>;
	using ToValueType = std::remove_reference_t<To>;

	if constexpr (!TIsCastable<FromValueType>::Value || !TIsCastable<ToValueType>::Value)
	{
		return dynamic_cast<To>(Arg);
	}
	else
	{
		static_assert(!TLosesQualifiersFromTo<FromValueType, ToValueType>::Value, "Conversion lose");

		static_assert(std::is_lvalue_reference_v<From> || std::is_rvalue_reference_v<To>, "Cannot dynamic_cast from an rvalue to a non-rvalue reference");

		return std::forward<To>(reinterpret_cast<To>)
	}

}