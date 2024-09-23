#pragma once
#include "Templates/MakeUnsigned.h"
#include "Templates/IsSigned.h"
#include <vector>
#include <set>
#include "HAL/Platform.h"
#include <memory>
#include <mutex>





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


	FORCEINLINE static constexpr SizeType GetTypeSize()
	{
		return sizeof(ElementType);
	}

	//std::vector<ElementType>::iterator begin()


	//decltype(auto) begin() const { return _vector.begin(); }
	//decltype(auto) end() const { return _vector.end(); }


	SizeType Add(ElementType&& Item) { _vector.push_back(Item); return Num() - 1; }
	SizeType Add(const ElementType& Item) { _vector.push_back(Item); return Num() - 1;}

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

	SizeType size() const { return _vector.size(); }

	SizeType Num() const { return _vector.size(); }

	void Empty() { return _vector.clear(); }

	void Reserve(SizeType Count) { _vector.reserve(Count); }

	void Resize(SizeType Count) { _vector.resize(Count);}

	decltype(auto) Erase(auto it) { return _vector.erase(it); }

	void Erase(SizeType Where) { auto It = _vector.begin() + Where; _vector.erase(It); }

	std::vector<ElementType>& GetVector() { return _vector; }


	ElementType PopBack() { if (_vector.size() == 0) return	NULL; auto it = _vector.end() - 1; auto result = *it; _vector.erase(it); return result; }

	template<typename OtherElementType>
	void Append(TArray<OtherElementType>&& Source) { _vector.insert(_vector.end(), Source._vector.begin(), Source._vector.end()); }

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


template <typename ElementType>
class TArrayThreadSafe : public TArray<ElementType>
{
	using SizeType = TArray<ElementType>::SizeType;
public:
	TArrayThreadSafe() { }

	TArrayThreadSafe(SizeType Size) : TArray<ElementType>(Size) {}

	TArrayThreadSafe(const ArrayReserve& InReserve) : TArray<ElementType>(InReserve) {}

	TArrayThreadSafe(std::initializer_list<ElementType> Elements) : TArray<ElementType>(Elements) {}

	TArrayThreadSafe(std::vector<ElementType> Elements) : TArray<ElementType>(Elements) {}

	template<std::size_t N>
	TArrayThreadSafe(ElementType(&Elements)[N]) : TArray<ElementType>(Elements) {}


	void Add(ElementType&& Item) { std::scoped_lock Lock(Mutex);  return TArray<ElementType>::Add(Item); }
	void Add(const ElementType& Item) { return TArray<ElementType>::Add(Item);}

	void Push(ElementType&& Item) { std::scoped_lock Lock(Mutex); TArray<ElementType>::Push(Item);}
	void Push(const ElementType& Item) { std::scoped_lock Lock(Mutex); TArray<ElementType>::Push(Item);}

	template <typename... ArgsType>
	decltype(auto) Emplace(ArgsType&&... Args) { std::scoped_lock Lock(Mutex); return TArray<ElementType>::Emplace(Args...);}

	template <typename... ArgsType>
	ElementType& Emplace_GetRef(ArgsType&&...Args)
	{
		std::scoped_lock Lock(Mutex);
		TArray<ElementType>::Emplace_GetRef(Args...);
		//return Last();
	}

	SizeType GetSize() const { std::scoped_lock Lock(Mutex); return TArray<ElementType>::Size(); }

	SizeType Num() const { std::scoped_lock Lock(Mutex); return TArray<ElementType>::Num();}

	void Empty() { std::scoped_lock Lock(Mutex); return TArray<ElementType>::Empty();}

	void Reserve(SizeType Count) { std::scoped_lock Lock(Mutex); TArray<ElementType>::Reserve(Count);}

	void Resize(SizeType Count) { std::scoped_lock Lock(Mutex); TArray<ElementType>::Resize(Count);}

	decltype(auto) Erase(auto it) { std::scoped_lock Lock(Mutex); return TArray<ElementType>::Erase(it);}


	ElementType PopBack() { std::scoped_lock Lock(Mutex); return TArray<ElementType>::PopBack();}

	template<typename OtherElementType>
	void Append(TArray<OtherElementType>&& Source) { std::scoped_lock Lock(Mutex); TArray<ElementType>::Append(Source);}

	decltype(auto) Insert(auto Where, auto It1, auto It2) { std::scoped_lock Lock(Mutex); return TArray<ElementType>::Insert(Where, It1, It2);}

	void Remove(const ElementType& Element)
	{
		std::scoped_lock Lock(Mutex);
		TArray<ElementType>::Remove(Element);
	}


	template<typename T>
	void RemovePtr(T* Element)
	{
		std::scoped_lock Lock(Mutex);
		TArray<ElementType>::RemovePtr(Element);
	}

	void RemovePtr(ElementType& Element)
	{
		std::scoped_lock Lock(Mutex);
		TArray<ElementType>::RemovePtr(Element);
	}


	bool IsValidIndex(SizeType Index) const
	{
		std::scoped_lock Lock(Mutex);
		return TArray<ElementType>::IsValidIndex(Index);
	}


	TArray<ElementType> operator+(const TArray<ElementType>& R)
	{
		std::scoped_lock Lock(Mutex);
		return TArray<ElementType>::operator+(R);
	}

private:
	std::mutex Mutex;


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
