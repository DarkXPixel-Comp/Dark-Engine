#include "D3D12Resources.h"
#include "D3D12Util.h"
#include "d3dx12.h"
#include "D3D12Device.h"
#include "D3D12CommandContext.h"



void FD3D12Heap::SetHeap(ID3D12Heap* InHeap, const TCHAR* InName, bool bForceGetGPUAddress)
{  
	//Heap = InHeap;
	//HeapName = InName;
	//HeapDesc = Heap->GetDesc();

	//SetName(InHeap, InName);

	//FD3D12Adapter* Adapter = GetParentDevice()->GetParentAdapter();

	//if (bForceGetGPUAddress && HeapDesc.Properties.Type == D3D12_HEAP_TYPE_DEFAULT)
	//{
	//	uint64 HeapSize = HeapDesc.SizeInBytes;
	//	ComPtr<ID3D12Resource> TempResource;

	//	const D3D12_RESOURCE_DESC BufDesc = CD3DX12_RESOURCE_DESC::Buffer(HeapSize, D3D12_RESOURCE_FLAG_NONE);

	//	DXCall(Adapter->GetD3DDevice()->CreatePlacedResource(Heap.Get(), 0, &BufDesc, 
	//		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&TempResource)));

	//	GPUVirtualAddress = TempResource->GetGPUVirtualAddress();
	//	
	//}





}

bool FD3D12ResourceBarrierBatcher::AddTransition(FD3D12Resource* pResource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource)
{
	//check(Before != After);

	//if (Barriers.Num() != 0)
	//{	
	//	const FD3D12ResourceBarrier& Last = Barriers.Last();

	//	if (Last.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION
	//		&& pResource->GetResource() == Last.Transition.pResource
	//		&& SubResource == Last.Transition.Subresource
	//		&& Before == Last.Transition.StateBefore
	//		&& After == Last.Transition.StateAfter)
	//	{
	//		Barriers.Remove(Last);
	//		return 0;
	//	}
	//}
	//

	//FD3D12ResourceBarrier& Barrier = Barriers.Emplace(CD3DX12_RESOURCE_BARRIER::Transition(pResource->GetResource(), Before,
	//	After, SubResource));


	return true;
}


FD3D12Resource::FD3D12Resource(
	FD3D12Device* ParentDevice,
	ID3D12Resource* InResource,
	D3D12_RESOURCE_STATES InInitialResourceState,
	const FD3D12ResourceDesc& InDesc,
	FD3D12Heap* InHeap,
	D3D12_HEAP_TYPE InHeapType
):
  FD3D12DeviceChild(ParentDevice),
	Resource(InResource),
	Heap(InHeap),
	Desc(InDesc),
	HeapType(InHeapType),
	DefaultResourceState(InInitialResourceState)
{
	if (Resource && Desc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
	{
		GPUVirtualAddress = Resource->GetGPUVirtualAddress();
	}

	DX12::SetName(this, TEXT("NULL"));



}

void FD3D12Buffer::UploadResourceData(FRHICommandListBase& InRHICmdList, const void* Buffer, uint32 SizeBuffer, D3D12_RESOURCE_STATES InDestinationState, const TCHAR* Name)
{
	check(Buffer);
	check(ResourceLocation.IsValid());

	const uint32 BufferSize = GetSize();

	check(BufferSize >= SizeBuffer);

	if (ResourceLocation.GetMappedBaseAddress())
	{
		void* MappedUploadData = ResourceLocation.GetMappedBaseAddress();
		FMemory::Memcpy(MappedUploadData, Buffer, SizeBuffer);
	}
	else
	{
		void* Data = nullptr;
		FD3D12ResourceLocation SourceResource(GetParentDevice());
		TRefCountPtr<ID3D12Resource> NewResource;
		
		const D3D12_HEAP_PROPERTIES HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		const D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(SizeBuffer, D3D12_RESOURCE_FLAG_NONE);
		
		GetParentDevice()->GetDevice()->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
			&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&NewResource));

		SourceResource.AsStandAlone(new FD3D12Resource(Parent, NewResource.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, ResourceDesc, nullptr, D3D12_HEAP_TYPE_UPLOAD), SizeBuffer, &HeapProps);;

		Data = SourceResource.GetMappedBaseAddress();
		check(Data);
		FMemory::Memcpy(Data, Buffer, SizeBuffer);
		

		FD3D12CommandContext& CommandContext = Parent->GetDefaultCommandContext();
		FD3D12Resource* Destination = ResourceLocation.GetResource();

		//CommandContext.GetCommandList().GetGraphicsCommandList()->CopyBufferRegion(Destination->GetResource())

		




	}

	



}
