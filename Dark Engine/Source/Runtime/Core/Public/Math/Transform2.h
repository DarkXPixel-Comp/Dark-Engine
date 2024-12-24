#pragma once

#include "Math/MathFwd.h"
#include "Math/Vector2.h"
#include "Math/Matrix2x2.h"
#include <type_traits>



namespace DE
{
	namespace Math
	{
		template <typename T>
		class TTransform2
		{
			static_assert(std::is_floating_point_v<T>, "T must be floating point");

		public:
			using FDark = T;
			using Vector2Type = DE::Math::TVector2<T>;
			using Matrix2Type = DE::Math::TMatrix2x2<T>;

			template<typename VType = float>
			TTransform2(const DE::Math::TVector2<VType>& Translation)
			{}

			Matrix2Type Mat;
			Vector2Type Translation;
		};


	}

}