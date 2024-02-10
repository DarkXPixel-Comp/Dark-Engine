#pragma once
#include <Core.h>
#include <Containers/DarkString.h>

#include <wrl.h>

#include <memory>

template <class T>
using TUniquePtr = std::unique_ptr<T>;

template <class ...Args>
using TSharedPtr = std::shared_ptr<Args...>;


template <class T>
using TRefCountPtr = Microsoft::WRL::ComPtr<T>;

template <class T>
using TWeakPtr = std::weak_ptr<T>;


template <class T>
TSharedPtr<T> MakeShareble(T* ptr)
{
	return TSharedPtr<T>(ptr);
	//return std::make_shared<T>(ptr);
}




//template <typename T>
//DEVELOP_CLASS(TUniquePtr)
//{
//public:
//	FString test()
//	{
//		return "test";
//	}
//
//};


