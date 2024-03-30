#include "D3D12ConstantBuffer.h"
#include "Templates/AlignTemplate.h"

FD3D12ConstantBuffer::FD3D12ConstantBuffer(FD3D12Device* InParent):
	FD3D12DeviceChild(InParent)
{

}

void FD3D12ConstantBuffer::UpdateConstant(const uint8* InData, uint16 Offset, uint16 InSize)
{
	const uint32 RequiredSize = (uint32)Offset + (uint32)InSize;
	if (Data.Num() < RequiredSize)
	{
		const uint32 NewSize = Align(RequiredSize, 512);
		Data.Resize(NewSize);
	}
	FMemory::Memcpy(Data.GetData() + Offset, InData, InSize);
}
