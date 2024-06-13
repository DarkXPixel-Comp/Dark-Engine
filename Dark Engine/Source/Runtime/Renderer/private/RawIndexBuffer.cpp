#include "RawIndexBuffer.h"


FRawStaticIndexBuffer::FRawStaticIndexBuffer()
{
}

void FRawStaticIndexBuffer::SetIndices(const TArray<uint32>& InIndices, uint16 InStride)
{
	uint32 NumIndices = InIndices.Num();
	Stride = InStride;
	CachedNumIndices = NumIndices;

	IndexStorage.Empty();
	IndexStorage.Resize(NumIndices * Stride);

	if (Stride == 4)
	{
		check(IndexStorage.Num() == InIndices.Num() * 4);
		FMemory::Memcpy(IndexStorage.GetData(), InIndices.GetData(), IndexStorage.Num());
	}
	else if (Stride == 2)
	{
		check(IndexStorage.Num() == InIndices.Num() * 2);
		uint16* DestIndices16Bit = (uint16*)IndexStorage.GetData();

		for (uint32 i = 0; i < NumIndices; ++i)
		{
			DestIndices16Bit[i] = static_cast<uint16>(InIndices[i]);
		}
	}
	else
	{
		check(false);
	}
}

