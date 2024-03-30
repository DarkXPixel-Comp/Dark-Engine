#pragma once
#include <stdlib.h>

#define DE_ARRAY_COUNT(arr) (_countof(arr))

template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T&> { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };



template <typename T>
typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;

	
	return (CastType&&)Obj;
}



class FNoncopyble
{
protected:
	FNoncopyble() {}
	~FNoncopyble() {}
private:
	FNoncopyble(const FNoncopyble&);
	FNoncopyble& operator=(const FNoncopyble&);


};