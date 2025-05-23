#pragma once
#include "Platform/Platform.h"


namespace DE::Math
{
	template <typename T> struct TVector2;
	template <typename T> class TTransform2;
	template <typename T> struct TRect;
	template <typename T> struct TIntPoint;
}

namespace ispc
{
	struct FVector2;
	struct FTransform2;
	struct FRect;
	struct FIntPoint;
}

using FVector2 = DE::Math::TVector2<double>;
using FTransform2 = DE::Math::TTransform2<double>;
using FRect = DE::Math::TRect<double>;

using FVector2d = DE::Math::TVector2<double>;
using FTransform2d = DE::Math::TTransform2<double>;
using FRect4d = DE::Math::TRect<double>;

using FVector2f = DE::Math::TVector2<float>;
using FTransform2f = DE::Math::TVector2<float>;
using FRect4f = DE::Math::TRect<float>;

using FIntRect = DE::Math::TRect<int32>;

using FIntPoint = DE::Math::TIntPoint<int32>;

using FInt32Point = DE::Math::TIntPoint<int32>;
using FInt64Point = DE::Math::TIntPoint<int64>;

using FUint32Point = DE::Math::TIntPoint<uint32>;
using FUint64Point = DE::Math::TIntPoint<uint64>;
