#pragma once
#include "Platform/Platform.h"
#include <cassert>
#include "Memory/MemoryCore.h"

template<class T>
class TSharedRef
{
public:

private:
	template<class OtherType>
	void Init(OtherType* InObject)
	{
		assert(InObject != nullptr);


		Object = MakeShareble(InObject);
	}

	std::shared_ptr<T> Object;
};
