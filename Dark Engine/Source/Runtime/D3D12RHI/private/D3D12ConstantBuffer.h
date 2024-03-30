#pragma once
#include "D3D12RHICommon.h"



class FD3D12ConstantBuffer : FD3D12DeviceChild
{
public:
	FD3D12ConstantBuffer(FD3D12Device* InParent);

	void UpdateConstant(const uint8* Data, uint16 Offset, uint16 InSize);

private:
	TArray<uint8> Data;



};