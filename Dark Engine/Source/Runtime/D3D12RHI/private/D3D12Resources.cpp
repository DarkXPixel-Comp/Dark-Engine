#include "D3D12Resources.h"
#include "D3D12Util.h"
#include "d3dx12.h"



void FD3D12Heap::SetHeap(ID3D12Heap* InHeap, const TCHAR* InName, bool bForceGetGPUAddress)
{  
	Heap = InHeap;
	HeapName = InName;
	HeapDesc = Heap->GetDesc();

	SetName(InHeap, InName);

	FD3D12Adapter* Adapter = GetParentDevice()->GetParentAdapter();

	if (bForceGetGPUAddress && HeapDesc.Properties.Type == D3D12_HEAP_TYPE_DEFAULT)
	{
		uint64 HeapSize = HeapDesc.SizeInBytes;
		ComPtr<ID3D12Resource> TempResource;

		const D3D12_RESOURCE_DESC BufDesc = CD3DX12_RESOURCE_DESC::Buffer(HeapSize, D3D12_RESOURCE_FLAG_NONE);

		DXCall(Adapter->GetD3DDevice()->CreatePlacedResource(Heap.Get(), 0, &BufDesc, 
			D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&TempResource)));

		GPUVirtualAddress = TempResource->GetGPUVirtualAddress();
		
	}





}