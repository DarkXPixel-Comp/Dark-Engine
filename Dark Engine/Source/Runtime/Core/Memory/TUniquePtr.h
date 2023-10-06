#pragma once
#include <Core/Core.h>
#include <Containers/DarkString.h>

#include <memory>

template <class T>
using TUniquePtr = std::unique_ptr<T>;




template <typename T>
DEVELOP_CLASS(TUniquePtr)
{
public:
	FString test()
	{
		return "test";
	}

};


