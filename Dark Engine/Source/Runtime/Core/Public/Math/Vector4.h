#pragma once	 
#include <type_traits>


namespace DE
{
	namespace Math
	{
		template<typename T>
		struct TVector4
		{
			static_assert(std::is_floating_point_v<T>, "T must be floating point");
		public:
			using FDark = T;
			union
			{
				struct
				{
					T X;
					T Y;
					T Z;
					T W;
				};
				T XYZW[4];
			};

			/*explicit TVector4(void):
				X(0), Y(0), Z(0), W(0)
			{

			}*/

			explicit TVector4(T InX = 0, T InY = 0, T InZ = 0, T InW = 0) :
				X(InX), Y(InY), Z(InZ), W(InW)
			{
			}


		};
	}
}