#pragma once
#include <functional>
#include "Misc/Delegate.h"

template <typename T>
class TAttribute
{
public:
	using FGetter = std::function<T()>;

	//DECLARE_DELEGATE_RetVal(T, FGetter);


	TAttribute()
		: Value() 
		, bIsSet(false)
		, Getter()
	{
	}

	template< typename OtherType >
	TAttribute(const OtherType& InInitialValue)
		: Value(static_cast<T>(InInitialValue))
		, bIsSet(true)
		, Getter()
	{
	}

	TAttribute(T&& InInitialValue)
		: Value(std::move(InInitialValue))
		, bIsSet(true)
		, Getter()
	{
	}

	template< class SourceType, typename Method >
	TAttribute(SourceType InUserObject, Method InMethod)
		: Value()
		, bIsSet(true)
	{
		//Getter.Bind(InUserObject, InMethod);
		Getter = std::bind(InMethod, InUserObject);
	}

	template< typename OtherType >
	void Set(const OtherType& InNewValue)
	{
		Getter = FGetter();
		Value = InNewValue;
		bIsSet = true;
	}

	void Set(T&& InNewValue)
	{
		Getter = FGetter();
		Value = std::move(InNewValue);
		bIsSet = true;
	}

	bool IsSet() const
	{
		return bIsSet;
	}


	const T& Get() const
	{
		if (Getter)
		{
			Value = Getter();
		}

		return Value;
	}

	const T& Get(const T& DefaultValue) const
	{
		return bIsSet ? Get() : DefaultValue;
	}

	void Bind(const FGetter& InGetter)
	{
		bIsSet = true;
		Getter = InGetter;
	}

	void Bind(FGetter&& InGetter)
	{
		bIsSet = true;
		Getter = std::move(InGetter);
	}


private:
	mutable T Value;
	bool bIsSet;

	FGetter Getter;



};

