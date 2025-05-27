#pragma once
#include <functional>
#include "Misc/Delegate.h"


template <typename T>
class TAttribute
{
public:
	//using FGetter = std::function<T()>;

	DECLARE_DELEGATE_RetVal(T, FGetter);


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
		Getter.Bind(InUserObject, InMethod);
	}

	template< typename OtherType >
	void Set(const OtherType& InNewValue)
	{
		Getter.Unbind();
		Value = InNewValue;
		bIsSet = true;
	}

	void Set(T&& InNewValue)
	{
		Getter.Unbind();
		Value = std::move(InNewValue);
		bIsSet = true;
	}

	bool IsSet() const
	{
		return bIsSet;
	}


	const T& Get() const
	{
		if (Getter.IsBound())
		{
			Value = Getter.BroadCast();
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

	bool IsBound() const
	{
		return Getter.IsBound();
	}

	bool IdenticalTo(const TAttribute& InOther) const
	{
		const bool bIsBound = IsBound();

		if (bIsBound == InOther.IsBound())
		{
			if (bIsBound)
			{
				return Getter.GetHandle() == InOther.Getter.GetHandle();
			}
			else
			{
				return IsSet() == InOther.bIsSet() && Value == InOther.Value;
			}
		}
		return false;
	}

private:
	mutable T Value;
	bool bIsSet;

	FGetter Getter;



};

