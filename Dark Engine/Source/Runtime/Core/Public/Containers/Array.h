#pragma once
#include "Templates/MakeUnsigned.h"
#include "Templates/IsSigned.h"
#include "Logging/Logger.hpp"
#include <vector>
#include "HAL/Platform.h"


#include "Templates/ChooseClass.h"



template <typename T>
using TArray = std::vector<T>;




namespace Develop
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

		typedef typename TChooseClass<AllocatorType::NeedsElementType,
			typename AllocatorType::template ForElementType<ElementType>,
			typename AllocatorType::ForAnyElementType
		>::Result ElementAllocatorType;

	/*	typedef typename TChooseClass<
			AllocatorType::NeedsElementType,
			typename AllocatorType::template ForElementType<ElementType>,
			typename AllocatorType::ForAnyElementType
		>::Result ElementAllocatorType;*/

		static_assert(TIsSigned<SizeType>::Value, "TArray only supports signed index types");
		

		//FORCEINLINE TArray()
		//	: ArrayNum(0),
		//	ArrayMax(AllocatorInstance.GetInitialCapacity())
		//{}

		FORCEINLINE TArray(const ElementType* Ptr, SizeType Count)
		{
			if(Count < 0)
			{
				Logger::log(TEXT("Trying to resize TArray to an invalid size of %llu"), LOGGER_ERROR);
			}

			check(Ptr != nullptr || Count == 0);

			CopyToEmpty(Ptr, Count, 0);
		}


















		SizeType AllocatorCalculateSlackReserve(SizeType NewArrayMax)
		{
	/*		if constexpr (TAllocatorTraits<AllocatorType>::SupportsElementAlignment)
			{
				return AllocatorInstance.CalculateSlackReserve(NewArrayMax, sizeof(ElementType), alignof(ElementType));
			}
			else
			{
				return AllocatorInstance.CalculateSlackReserve(NewArrayMax, sizeof(ElementType));
			}*/
		}





		FORCENOINLINE void ResizeForCopy(SizeType NewMax, SizeType PrevMax)
		{
			if (NewMax)
			{
				NewMax = AllocatorCalculateSlackReserve(NewMax);
			}
			if (NewMax > PrevMax)
			{
				AllocatorResizeAllocation(0, NewMax);
				ArrayMax = NewMax;
			}
			else
			{
				ArrayMax = PrevMax;
			}
		}








	protected:
		ElementAllocatorType AllocatorInstance;
		SizeType ArrayNum;
		SizeType ArrayMax;



		template <typename OtherElementType, typename OtherSizeType>
		void CopyToEmpty(const OtherElementType* OtherData, OtherSizeType OtherNum, SizeType PrevMax)
		{
			//SizeType NewNum = (SizeType)OtherNum;
			//checkf((OtherSizeType)NewNum == OtherNum, TEXT("Invalid number of elements to add to this array type: %lld"), (long long)NewNum);

			//ArrayNum = NewNum;
			//if (OtherNum || PrevMax)
			//{
			//	ResizeForCopy(NewNum, PrevMax);
			//	ConstructItems<ElementType>(GetData(), OtherData, OtherNum);
			//}
			//else
			//{
			//	ArrayMax = AllocatorInstance.GetInitialCapacity();
			//}
		}

	};

}
