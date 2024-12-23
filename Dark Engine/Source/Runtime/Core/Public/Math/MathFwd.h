#pragma once
#include "Platform/Platform.h"


namespace DE::Math
{
	template <typename T> struct TVector2;
}

namespace ispc
{
	struct FVector2;
}

using FVector2 = DE::Math::TVector2<double>;


using FVector2d = DE::Math::TVector2<double>;

using FVector2f = DE::Math::TVector2<float>;