#pragma once
#include <vector>
#include "Templates/MakeUnsigned.h"
#include "Templates/IsSigned.h"


template <typename T>
using TArray = std::vector<T>;




namespace DarkDevelop
{
	template<typename InElementType, typename InAllocatorType>
	class TArray
	{
		template<typename OtherInElementType, typename OtherAllocator>
		friend class TArray;

	public:
		typedef typename InAllocatorType::SizeType SizeType;
		typedef InElementType ElementType;
		typedef InAllocatorType AllocatorType;

	private:
		using USizeType = typename TMakeUnsigned<SizeType>::Type;

		static_assert(TIsSigned<SizeType>::Value, "TArray only supports signed index types");
		



	};

}
