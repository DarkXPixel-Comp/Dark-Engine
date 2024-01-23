#include "D3D12Device.h"


FD3D12Device::FD3D12Device(FD3D12Adapter* InAdapter) :
	FD3D12AdapterChild(InAdapter)
{
	for (uint32 HeapType = 0; HeapType < (uint32)ERHIDescriptorHeapType::Count; ++HeapType)
	{

	}
	



}