#pragma once
#include "CoreTypes.h"
#include "Containers/Array.h"
#include "HAL/PlatformMath.h"



class TBitArray
{	   
public:
	explicit TBitArray() :
		NumBits(0)
	{}

	explicit TBitArray(bool bValue, uint64 InNumBits)
	{
		Init(bValue, InNumBits);
	}

	FORCEINLINE void Init(bool bValue, uint64 InNumBits)
	{
		Resize(bValue, true, bValue);


	}


	FORCEINLINE void Resize(uint64 InNumBits, bool bEmpty = false, bool bValue = false)
	{
		NumBits = InNumBits;
		uint64 SizeBitsArray = NumBits / 8 + (NumBits % 8 == 0 ? 0 : 1);

		Bits.Resize(SizeBitsArray);

		if (bEmpty)
		{
			for (uint64 i = 0; i < Bits.GetSize(); ++i)
			{
				Bits[i] = bValue ? 0xff : 0;
			}
		}
	}


	FORCEINLINE void SetBit(uint64 Index, bool bValue)
	{
		uint64 IndexBitsArray = Index / 8;
		uint64 IndexBitInByte = Index % 8;
		uint8 Byte = Bits[IndexBitsArray];
		uint8 Mask = FMath::Pow(2ull, IndexBitInByte);

		Byte = Byte & (~Mask);

		if (bValue)
		{
			Byte = Byte | Mask;
		}

		Bits[IndexBitsArray] = Byte;
	}

	FORCEINLINE bool GetBit(uint64 Index) const
	{
		return (*this)[Index];
	}

	FORCEINLINE bool operator[](uint64 Index) const
	{		
		check(Index < NumBits);
		uint64 IndexBitsArray = Index / 8;
		uint64 IndexBitInByte = Index % 8;
		uint8 Byte = Bits[IndexBitsArray];

		uint8 Mask = FMath::Pow(2ull, IndexBitInByte);

		return Byte & Mask;
	}


	FORCEINLINE int64 FindAndSetFirstZeroBit()
	{
		for (uint64 i = 0; i < NumBits; ++i)
		{								  
			if (!GetBit(i))
			{
				SetBit(i, true);
				return i;
			}
		}
		return -1;
	}


private:
	uint64 NumBits;
	TArray<uint8> Bits;


};