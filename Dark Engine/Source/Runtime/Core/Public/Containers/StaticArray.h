#pragma once


#include "array"



template <typename ElementType, uint32 ElementCount>
class TStaticArray : public TArray<ElementType>
{
public:
	TStaticArray() : TArray<ElementType>(ElementCount) {}

};