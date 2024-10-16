#pragma once
#include "Templates/MakeUnsigned.h"
#include "Templates/IsSigned.h"
#include <vector>
#include <set>
#include "HAL/Platform.h"
#include <memory>





template <class ...Args>
using TSet = std::set<Args...>;



struct ArrayReserve
{
	ArrayReserve(uint64 InReserve): Reserve(InReserve)
	{}

	uint64 Reserve;
};


template <typename ElementType>
class TArray
{
	using SizeType = uint64;

public:
	TArray() { }

	TArray(SizeType Size) : _vector(Size) {}

	TArray(const ArrayReserve& InReserve) { Reserve(InReserve.Reserve); }

	TArray(std::initializer_list<ElementType> Elements) : _vector(Elements) {}

	TArray(std::vector<ElementType> Elements) : _vector(Elements) {}

	template<std::size_t N>
	TArray(ElementType (&Elements)[N]) :_vector(N) 
	{
		memcpy(_vector.data(), Elements, N);
	}
		
	/*TArray(ElementType* Elements, SizeType N) :_vector(N)
	{
		memcpy(_vector.data(), Elements, N);
	}*/

	constexpr decltype(auto) begin() const { return _vector.begin(); }
	constexpr decltype(auto) end() const { return _vector.end(); }

	constexpr decltype(auto) begin() { return _vector.begin(); }
	constexpr decltype(auto) end() { return _vector.end(); }


	//std::vector<ElementType>::iterator begin()


	//decltype(auto) begin() const { return _vector.begin(); }
	//decltype(auto) end() const { return _vector.end(); }


	void Add(ElementType&& Item) { return _vector.push_back(Item); }
	void Add(const ElementType& Item) { return _vector.push_back(Item); }

	void Push(ElementType&& Item) { _vector.push_back(Item); }
	void Push(const ElementType& Item) { _vector.push_back(Item); }

	template <typename... ArgsType>
	decltype(auto) Emplace(ArgsType&&... Args) { return _vector.emplace_back(std::forward<ArgsType>(Args)...); }

	template <typename... ArgsType>
	ElementType& Emplace_GetRef(ArgsType&&...Args)
	{
		_vector.emplace_back(std::forward<ArgsType>(Args)...);
		//return Last();
	}


	auto GetData() { return _vector.data(); }
	const ElementType* GetData() const { return _vector.data(); }
	//const auto GetData() { return _vector.data(); }

	SizeType GetSize() const { return _vector.size(); }

	SizeType Num() const { return _vector.size(); }

	void Empty() { return _vector.clear(); }

	void Reserve(SizeType Count) { _vector.reserve(Count); }

	void Resize(SizeType Count) { _vector.resize(Count);}

	decltype(auto) Erase(auto it) { return _vector.erase(it); }

	std::vector<ElementType>& GetVector() { return _vector; }


	ElementType PopBack() { if (_vector.size() == 0) return nullptr; auto it = _vector.end() - 1; auto result = *it; _vector.erase(it); return result; }

	template<typename OtherElementType>
	void Append(TArray<OtherElementType>&& Source) { _vector.append_range(Source); }

	decltype(auto) Insert(auto Where, auto It1, auto It2) { return _vector.insert(Where, It1, It2); }

	void Remove(const ElementType& Element) 
	{
		auto it = std::find(_vector.begin(), _vector.end(), Element);
		if (it != _vector.end())
		{
			_vector.erase(it);
		}
	}
	template<typename T>
	//void RemovePtr(const decltype(std::shared_ptr<T>)& Element)
	//{
	//	Remove(Element);
	//}
	void RemovePtr(const std::shared_ptr<T>& Element)
	{
		Remove(Element);
	}

	template<typename T>
	void RemovePtr(T* Element)
	{
		ElementType* BaseElement = _vector.data();

		for (uint64 i = 0; i < _vector.size(); ++i)
		{
			if ((BaseElement + i)->get() == Element)
			{
				_vector.erase(_vector.begin() + i);
				return;
			}

		}
	}

	void RemovePtr(ElementType& Element)
	{
		ElementType* BaseElement = _vector.data();
		ElementType* OtherElement = &Element;
		for (size_t i = 0; i < _vector.size(); ++i)
		{
			if ((BaseElement + i) == OtherElement)
			{
				_vector.erase(_vector.begin() + i);
				return;
			}

		}
	}


	ElementType& Last() { return _vector[_vector.size() - 1]; }
	ElementType& First() { return _vector[0]; }

	//void AddZeroed(uint32 Num) {}



	const ElementType& operator[](SizeType Index) const { return _vector[Index]; }
	ElementType& operator[](SizeType Index) { return _vector[Index]; }

	bool IsValidIndex(SizeType Index) const { return Index < _vector.size(); }


	TArray<ElementType> operator+(const TArray<ElementType>& R)
	{
		TArray<ElementType>	Result(_vector);
		Result.Insert(Result.end(), R.begin(), R.end());

		return Result;
	}



	operator bool() const
	{
		return Num() != 0;
	}

private:
	std::vector<ElementType> _vector;

	friend struct std::hash<TArray<ElementType>>;
};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

inline void hash_without_hash_combine(std::size_t& seed, const std::size_t& hash)
{
	seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T>
struct std::hash<TArray<T>>
{
	std::size_t operator()(const TArray<T>& Key) const
	{
		std::size_t Result = 0;
		for (const auto& i : Key)
		{
			hash_combine(Result, i);
		}
		return Result;
	}
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


