#pragma once


#include <memory>

template <class ...Args>
using TSharedPtr = std::shared_ptr<Args...>;

template<typename T>
using TUniquePtr = std::unique_ptr<T>;

template<typename T>
using TWeakPtr = std::weak_ptr<T>;

template <class T>
TSharedPtr<T> MakeShareble(T* ptr)
{
	return TSharedPtr<T>(ptr);
}

template <class T, typename ...Args>
TSharedPtr<T> MakeShareble(Args... args)
{
	return std::make_shared<T>(args...);
}

template <class Base, class Derived>
TSharedPtr<Base> StaticCastSharedPtr(const TSharedPtr<Derived>& ptr)
{
	return std::static_pointer_cast<Base>(ptr);
}

template <class T>
TUniquePtr<T> MakeUnique(T* ptr)
{
	return TUniquePtr<T>(ptr);
}