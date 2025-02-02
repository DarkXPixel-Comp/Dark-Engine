#pragma once
#include <memory>
#include <functional>
#include <vector>

#include "Platform/Platform.h"


struct ArrayReserve
{
	ArrayReserve(uint64 InReserve) : Reserve(InReserve)
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
	TArray(ElementType(&Elements)[N]) : _vector(N)
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


	FORCEINLINE bool Contains(const ElementType& Other) const
	{
		auto It = std::find(begin(), end(), Other);
		return It != end();
	}

	FORCEINLINE bool Contains(std::function<bool(const ElementType&)> func) const
	{
		for (const auto& i : *this)
		{
			if (func(i))
				return true;
		}
		return false;
	}

	FORCEINLINE void Sort()
	{
		std::sort(begin(), end());
	}

	FORCEINLINE void Sort(std::function<bool(const ElementType&, const ElementType&)> func)
	{
		std::sort(begin(), end(), func);
	}

	FORCEINLINE static constexpr SizeType GetTypeSize()
	{
		return sizeof(ElementType);
	}

	//std::vector<ElementType>::iterator begin()


	//decltype(auto) begin() const { return _vector.begin(); }
	//decltype(auto) end() const { return _vector.end(); }


	SizeType Add(ElementType&& Item) { _vector.push_back(Item); return Num() - 1; }
	SizeType Add(const ElementType& Item) { _vector.push_back(Item); return Num() - 1; }

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

	void Resize(SizeType Count) { _vector.resize(Count); }

	decltype(auto) Erase(auto it) { return _vector.erase(it); }

	void Erase(SizeType Where) { auto It = _vector.begin() + Where; _vector.erase(It); }

	std::vector<ElementType>& GetVector() { return _vector; }


	ElementType PopBack() { if (_vector.size() == 0) return	NULL; auto it = _vector.end() - 1; auto result = *it; _vector.erase(it); return result; }

	template<typename OtherElementType>
	void Append(TArray<OtherElementType>&& Source) { _vector.insert(_vector.end(), Source._vector.begin(), Source._vector.end()); }

	decltype(auto) Insert(auto Where, auto It1, auto It2) { return _vector.insert(Where, It1, It2); }

	bool Has(const ElementType& Element)
	{
		auto it = std::find(_vector.begin(), _vector.end(), Element);
		return it != _vector.end();
	}

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
