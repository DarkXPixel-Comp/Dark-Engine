#pragma once
#include <type_traits>

// Defines all bitwise operators for enum classes so it can be (mostly) used as a regular flags enum
#define ENUM_CLASS_FLAGS(Enum) \
	inline           Enum& operator|=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
	inline           Enum& operator&=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
	inline           Enum& operator^=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator| (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator& (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator^ (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
	inline constexpr bool  operator! (Enum  E)             { return !(__underlying_type(Enum))E; } \
	inline constexpr Enum  operator~ (Enum  E)             { return (Enum)~(__underlying_type(Enum))E; }

// Friends all bitwise operators for enum classes so the definition can be kept private / protected.
#define FRIEND_ENUM_CLASS_FLAGS(Enum) \
	friend           Enum& operator|=(Enum& Lhs, Enum Rhs); \
	friend           Enum& operator&=(Enum& Lhs, Enum Rhs); \
	friend           Enum& operator^=(Enum& Lhs, Enum Rhs); \
	friend constexpr Enum  operator| (Enum  Lhs, Enum Rhs); \
	friend constexpr Enum  operator& (Enum  Lhs, Enum Rhs); \
	friend constexpr Enum  operator^ (Enum  Lhs, Enum Rhs); \
	friend constexpr bool  operator! (Enum  E); \
	friend constexpr Enum  operator~ (Enum  E);


template <typename Enum>
constexpr bool EnumHasAnyFlags(Enum Flags, Enum Contains)
{
	using UnderlyingType = __underlying_type(Enum);
	return ((UnderlyingType)Flags & (UnderlyingType)Contains) != 0;
}