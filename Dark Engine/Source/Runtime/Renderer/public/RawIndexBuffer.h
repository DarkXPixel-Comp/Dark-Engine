#pragma once
#include "RenderResource.h"


class FRawStaticIndexBuffer : public FIndexBuffer
{
public:
	FRawStaticIndexBuffer();

	void SetIndices(const TArray<uint32>& InIndices, uint16 InStride);


private:
	TArray<uint8> IndexStorage;
	uint16 Stride;
	uint32 CachedNumIndices = MAX_uint32;

};