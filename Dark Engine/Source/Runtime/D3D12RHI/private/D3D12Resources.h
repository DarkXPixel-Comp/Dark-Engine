#pragma once
#include <d3d12.h>
#include "Memory/TUniquePtr.h"

#include "D3D12RHICommon.h"




class FD3D12Heap : public FD3D12DeviceChild
{
public:
	FD3D12Heap(FD3D12Device* InDevice);

	ID3D12Heap* GetHeap() const;

	void SetHeap(ID3D12Heap* InHeap, const TCHAR* InName, bool bForceGetGPUAddress);

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return GPUVirtualAddress; }



private:
	ComPtr<ID3D12Heap> Heap;
	TCHAR* HeapName;
	D3D12_HEAP_DESC HeapDesc;
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress = 0;




};







class FD3D12Resource
{
	TRefCountPtr<ID3D12Resource> Resource;


	TRefCountPtr<FD3D12Heap> Heap;




};