#pragma once


#include "Containers/Array.h"



template <typename ElementType, uint32 ElementCount>
class TStaticArray : public TArray<ElementType>
{
public:
	TStaticArray() : TArray<ElementType>(ElementCount) {}

};