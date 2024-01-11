#pragma once
#include <Core.h>
#include <Containers/DarkString.h>

#include <memory>

template <class T>
using TUniquePtr = std::unique_ptr<T>;

template <class T>
using TSharedPtr = std::shared_ptr<T>;




template <typename T>
DEVELOP_CLASS(TUniquePtr)
{
public:
	FString test()
	{
		return "test";
	}

};


