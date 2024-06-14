#pragma once
#include "RenderResource.h"


class FRawStaticIndexBuffer : public FIndexBuffer
{
public:
	FRawStaticIndexBuffer();

	void SetIndices(const TArray<uint32>& InIndices, uint16 InStride);


	FORCEINLINE uint32 GetNumIndices() const
	{
		return CachedNumIndices >= 0 ? CachedNumIndices : (Stride == 4) ? (IndexStorage.Num() / 4) : (IndexStorage.Num() / 2);
	}

	virtual void InitRHI(FRHICommandListImmediate& RHICmdList) override;

private:
	TArray<uint8> IndexStorage;
	uint16 Stride;
	uint32 CachedNumIndices = MAX_uint32;

};