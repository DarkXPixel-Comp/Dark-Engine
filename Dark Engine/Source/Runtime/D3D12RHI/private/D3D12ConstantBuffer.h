#pragma once
#include "D3D12RHICommon.h"
#include "D3D12Resources.h"



class FD3D12ConstantBuffer : FD3D12DeviceChild
{
public:
	FD3D12ConstantBuffer(FD3D12Device* InParent);

	void UpdateConstant(const uint8* Data, uint16 Offset, uint16 InSize);

	bool Version(class FD3D12ResourceLocation*& BufferOut, bool bDiscardConstants);

	void Reset() { CurrentUpdateSize = 0; }

private:
	TArray<uint8> Data;
	FD3D12ResourceLocation ResourceLocation;
	bool bDirty = true;
	uint32 CurrentUpdateSize = 0;
	uint32 TotalUpdateSize = 0;



};