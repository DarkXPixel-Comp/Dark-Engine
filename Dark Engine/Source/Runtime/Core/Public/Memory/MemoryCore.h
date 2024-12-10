#pragma once


#include <memory>

template <class ...Args>
using TSharedPtr = std::shared_ptr<Args...>;

template<typename T>
using TUniquePtr = std::unique_ptr<T>;

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

template <class T>
TUniquePtr<T> MakeUnique(T* ptr)
{
	return TUniquePtr<T>(ptr);
}