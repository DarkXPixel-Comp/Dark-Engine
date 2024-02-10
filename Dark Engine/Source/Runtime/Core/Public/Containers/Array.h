#pragma once
#include "Templates/MakeUnsigned.h"
#include "Templates/IsSigned.h"
#include "Logging/Logger.hpp"
#include <vector>
#include <set>
#include "HAL/Platform.h"


#include "Templates/ChooseClass.h"



template <class ...Args>
using TSet = std::set<Args...>;



template <typename ElementType>
class TArray
{
	using SizeType = uint64;

public:
	TArray() {}

	TArray(SizeType Size) : _vector(Size) {}

	TArray(std::initializer_list<ElementType> Elements) : _vector(Elements) {}

	TArray(std::vector<ElementType> Elements) : _vector(Elements) {}


	constexpr decltype(auto) begin() const { return _vector.begin(); }
	constexpr decltype(auto) end() const { return _vector.end(); }


	void Add(ElementType&& Item) { return _vector.push_back(Item); }
	void Add(const ElementType& Item) { return _vector.push_back(Item); }

	void Push(ElementType&& Item) { _vector.push_back(Item); }
	void Push(const ElementType& Item) { _vector.push_back(Item); }

	template <typename... ArgsType>
	decltype(auto) Emplace(ArgsType&&... Args) { return _vector.emplace_back(std::forward<ArgsType>(Args)...); }

	auto GetData() { return _vector.data(); }
	//const auto GetData() { return _vector.data(); }

	SizeType GetSize() { return _vector.size(); }

	void Empty() { return _vector.clear(); }

	void Reserve(SizeType Count) { _vector.reserve(Count); }

	void Resize(SizeType Count) { _vector.resize(Count);}

	decltype(auto) Erase(auto it) { return _vector.erase(it); }

	std::vector<ElementType>& GetVector() { return _vector; }

	decltype(auto) Insert(auto Where, auto It1, auto It2) { return _vector.insert(Where, It1, It2); }



	const ElementType& operator[](SizeType Index) const { return _vector[Index]; }
	ElementType& operator[](SizeType Index) { return _vector[Index]; }


	TArray<ElementType> operator+(const TArray<ElementType>& R)
	{
		TArray<ElementType>	Result(_vector);
		Result.Insert(Result.end(), R.begin(), R.end());

		return Result;
	}
	






private:
	std::vector<ElementType> _vector;
};



//template <typename T>
//using TArray = std::vector<T>;





//
//namespace Develop
//{
//	template<typename InElementType, typename InAllocatorType>
//	class TTArray
//	{
//		template<typename OtherInElementType, typename OtherAllocator>
//		friend class TArray;
//
//	public:
//		typedef typename InAllocatorType::SizeType SizeType;
//		typedef InElementType ElementType;
//		typedef InAllocatorType AllocatorType;
//
//	private:
//		using USizeType = typename TMakeUnsigned<SizeType>::Type;
//
//		typedef typename TChooseClass<AllocatorType::NeedsElementType,
//			typename AllocatorType::template ForElementType<ElementType>,
//			typename AllocatorType::ForAnyElementType
//		>::Result ElementAllocatorType;
//
//	/*	typedef typename TChooseClass<
//			AllocatorType::NeedsElementType,
//			typename AllocatorType::template ForElementType<ElementType>,
//			typename AllocatorType::ForAnyElementType
//		>::Result ElementAllocatorType;*/
//
//		static_assert(TIsSigned<SizeType>::Value, "TArray only supports signed index types");
//		
//
//		//FORCEINLINE TArray()
//		//	: ArrayNum(0),
//		//	ArrayMax(AllocatorInstance.GetInitialCapacity())
//		//{}
//
//		FORCEINLINE TTArray(const ElementType* Ptr, SizeType Count)
//		{
//			if(Count < 0)
//			{
//				Logger::log(TEXT("Trying to resize TArray to an invalid size of %llu"), LOGGER_ERROR);
//			}
//
//			check(Ptr != nullptr || Count == 0);
//
//			CopyToEmpty(Ptr, Count, 0);
//		}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//		SizeType AllocatorCalculateSlackReserve(SizeType NewArrayMax)
//		{
//	/*		if constexpr (TAllocatorTraits<AllocatorType>::SupportsElementAlignment)
//			{
//				return AllocatorInstance.CalculateSlackReserve(NewArrayMax, sizeof(ElementType), alignof(ElementType));
//			}
//			else
//			{
//				return AllocatorInstance.CalculateSlackReserve(NewArrayMax, sizeof(ElementType));
//			}*/
//		}
//
//
//
//
//
//		FORCENOINLINE void ResizeForCopy(SizeType NewMax, SizeType PrevMax)
//		{
//			if (NewMax)
//			{
//				NewMax = AllocatorCalculateSlackReserve(NewMax);
//			}
//			if (NewMax > PrevMax)
//			{
//				AllocatorResizeAllocation(0, NewMax);
//				ArrayMax = NewMax;
//			}
//			else
//			{
//				ArrayMax = PrevMax;
//			}
//		}
//
//
//
//
//
//
//
//
//	protected:
//		ElementAllocatorType AllocatorInstance;
//		SizeType ArrayNum;
//		SizeType ArrayMax;
//
//
//
//		template <typename OtherElementType, typename OtherSizeType>
//		void CopyToEmpty(const OtherElementType* OtherData, OtherSizeType OtherNum, SizeType PrevMax)
//		{
//			SizeType NewNum = (SizeType)OtherNum;
//			checkf((OtherSizeType)NewNum == OtherNum, TEXT("Invalid number of elements to add to this array type: %lld"), (long long)NewNum);
//
//			ArrayNum = NewNum;
//			if (OtherNum || PrevMax)
//			{
//				ResizeForCopy(NewNum, PrevMax);
//				//ConstructItems<ElementType>(GetData(), OtherData, OtherNum);
//			}
//			else
//			{
//				ArrayMax = AllocatorInstance.GetInitialCapacity();
//			}
//		}
//
//	};
//
//}


