#include "D3D12ConstantBuffer.h"
#include "Templates/AlignTemplate.h"
#include "D3D12Resources.h"
#include <D3D12Util.h>
#include "D3D12Device.h"

FD3D12ConstantBuffer::FD3D12ConstantBuffer(FD3D12Device* InParent):
	FD3D12DeviceChild(InParent),
	ResourceLocation(InParent)
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
	CurrentUpdateSize = FMath::Max(RequiredSize, CurrentUpdateSize);
	bDirty = true;
}

bool FD3D12ConstantBuffer::Version(FD3D12ResourceLocation*& BufferOut, bool bDiscardConstants)
{
	if (CurrentUpdateSize == 0)
	{
		return false;
	}

	if (bDiscardConstants)
	{
		TotalUpdateSize = CurrentUpdateSize;
	}
	else
	{
		TotalUpdateSize = FMath::Max(CurrentUpdateSize, TotalUpdateSize);
	}

	if (!ResourceLocation.IsValid() || TotalUpdateSize > ResourceLocation.GetSize())
	{
		ResourceLocation.Clear();
		const D3D12_RESOURCE_DESC BufferDesc = CD3DX12_RESOURCE_DESC::Buffer(TotalUpdateSize, D3D12_RESOURCE_FLAG_NONE);
		const D3D12_HEAP_PROPERTIES HeapProperties = D3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		TRefCountPtr<ID3D12Resource> Resource;
		DXCall(Parent->GetDevice()->CreateCommittedResource(&HeapProperties,
			D3D12_HEAP_FLAG_NONE, &BufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&Resource)));

		ResourceLocation.AsStandAlone(new FD3D12Resource(Parent, Resource.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, BufferDesc, nullptr, D3D12_HEAP_TYPE_UPLOAD), TotalUpdateSize, nullptr);
	}
	void* pData = ResourceLocation.GetMappedBaseAddress();

	FMemory::Memcpy(pData, Data.GetData(), TotalUpdateSize);

	bDirty = false;

	BufferOut = &ResourceLocation;

	return true;
}
