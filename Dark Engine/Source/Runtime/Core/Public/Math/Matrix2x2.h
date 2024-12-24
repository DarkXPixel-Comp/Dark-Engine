#pragma once
#include "Math/Vector2.h"



namespace DE
{
	namespace Math
	{
		template<typename T>
		class TMatrix2x2
		{
			static_assert(std::is_floating_point_v<T>, "T must be floating point");
		public:
			using Vector2Type = DE::Math::TVector2<T>;
			T M[2][2];

			TMatrix2x2()
			{
				M[0][0] = 1; M[0][1] = 0;
				M[1][0] = 0; M[1][1] = 1;
			}

			TMatrix2x2(T M00, T M01, T M10, T M11)
			{
				M[0][0] = M00; M[0][1] = M01;
				M[1][0] = M10; M[1][1] = M11;
			}





		};


	}
}


