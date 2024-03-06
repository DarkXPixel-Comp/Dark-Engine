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
	//TSharedPtr<T> Result = std::make_shared<T>(ptr);
	return TSharedPtr<T>(ptr);
	//return std::make_shared<T>(ptr);
}
template <class T, typename ...Args>
TSharedPtr<T> MakeShareble(Args... args)
{
	return std::make_shared<T>(args...);
}

//template <class T>
//TSharedPtr<T> MakeSharebleThis(T* ptr)
//{
//	struct Temp : public T, public std::enable_shared_from_this<T>
//	{
//		FORCEINLINE std::shared_ptr<T> Create()
//		{
//			return shared_from_this<T>();
//		}
//	};
//
//	Temp* Result = reinterpret_cast<T*>(ptr);
//
//	return Result->Create();
//}


//template <class T>
//TSharedPtr<T> MakeSharebleThis(T* ptr)
//{
//	return MakeShareble(std::shared_from_t)
//}

template <class T, class F>
TSharedPtr<T> StaticCastSharedPtr(TSharedPtr<F> ptr)
{
	return std::static_pointer_cast<T>(ptr);
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


