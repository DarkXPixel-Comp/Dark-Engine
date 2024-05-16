#pragma once


#include "D3D12RHICommon.h"
#include "D3D12Resources.h"
#include "D3D12CommandList.h"
#include "Containers/UnordoredMap.h"


struct FD3D12Barrier
{
public:
	FD3D12Barrier() {}

	D3D12_BARRIER_TYPE BarrierType;

	union
	{
		struct
		{
			D3D12_BARRIER_SYNC BeforeSync;
			D3D12_BARRIER_SYNC AfterSync;
			
			D3D12_BARRIER_ACCESS BeforeAccess;
			D3D12_BARRIER_ACCESS AfterAccess;

			uint64 Size = UINT64_MAX;
			uint64 Offset = 0;
		} BufferBarrier;
	};
};


class FD3D12BarrierBatcher
{
public:
	FD3D12BarrierBatcher();

	void TransitionResource(FD3D12Buffer* InBuffer, D3D12_BARRIER_SYNC AfterSync,
		D3D12_BARRIER_ACCESS AfterAccess, uint64 Size = UINT64_MAX, uint64 Offset = 0);

	void TransitionResource(FD3D12Texture* InTexture, D3D12_BARRIER_SYNC AfterSync,
		D3D12_BARRIER_ACCESS AfterAccess, D3D12_BARRIER_LAYOUT AfterLayout, CD3DX12_BARRIER_SUBRESOURCE_RANGE Subresource);

	void TransitionTexture(FD3D12Resource* InTexture, D3D12_BARRIER_SYNC AfterSync,
		D3D12_BARRIER_ACCESS AfterAccess, D3D12_BARRIER_LAYOUT AfterLayout, CD3DX12_BARRIER_SUBRESOURCE_RANGE Subresource);

	void TransitionBuffer(FD3D12ResourceLocation& Resource, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess,
		uint64 Size = UINT64_MAX, uint64 Offset = 0);

	void FlushBarrierGroups(FD3D12CommandList& CmdList);

private:
	TMap<FD3D12Resource*, FD3D12Barrier> Barriers;

	//TMap<FD3D12Resource*, CD3DX12_BUFFER_BARRIER> BufferBarriers;

	TArray<std::pair<FD3D12Resource*, CD3DX12_BUFFER_BARRIER>> BufferBarriers;
	TArray<std::pair<FD3D12Resource*, CD3DX12_TEXTURE_BARRIER>> TextureBarriers;


};