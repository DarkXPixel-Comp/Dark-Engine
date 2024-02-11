#pragma once
#include "Templates/IsFloatingPoint.h"


template <typename T>
struct alignas(16) TMatrix
{
	static_assert(TIsFloatingPoint<T>::Value, "T must be float or double");

public:
	using FReal = T;

	alignas(16)	T Matrix[4][4];

	static const TMatrix Identity;







};