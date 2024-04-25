#pragma once
#include "D3D12RHICommon.h"
#include "D3D12ConstantBuffer.h"
#include "RHIDefines.h"
//#include "D3D12CommandContext.h"

class FD3D12CommandContext;

template<typename ResourceSlotMask>
struct FD3D12ResourceCache
{
	static FORCEINLINE void CleanSlot(ResourceSlotMask& SlotMask, uint32 SlotIndex)
	{
		SlotMask &= ~((ResourceSlotMask)1 << SlotIndex);
	}

	static FORCEINLINE void CleanSlots(ResourceSlotMask& SlotMask, uint32 NumSlots)
	{
		SlotMask &= ~(((ResourceSlotMask)1 << NumSlots) - 1);
	}

	static FORCEINLINE void DirtySlot(ResourceSlotMask& SlotMask, uint32 SlotIndex)
	{
		SlotMask |= ((ResourceSlotMask)1 << SlotIndex);
	}

	static FORCEINLINE bool IsSlotDirty(const ResourceSlotMask& SlotMask, uint32 SlotIndex)
	{
		return (SlotMask & ((ResourceSlotMask)1 << SlotIndex)) != 0;
	}


	ResourceSlotMask DirtySlotMask[ST_NumStandartTypes] = {};
};


struct FD3D12VertexBufferCache
{
	FD3D12VertexBufferCache()
	{
		Clear();
	}

	FORCEINLINE void Clear()
	{
		FMemory::Memzero(CurrentVertexBufferResources, sizeof(CurrentVertexBufferResources));
		FMemory::Memzero(CurrentVertexBufferViews, sizeof(CurrentVertexBufferViews));
		NumViews = 0;
	}


	D3D12_VERTEX_BUFFER_VIEW CurrentVertexBufferViews[32];
	class FD3D12ResourceLocation* CurrentVertexBufferResources[32];
	int32 NumViews = 0;

};


struct FD3D12IndexBufferCache
{
	FD3D12IndexBufferCache()
	{
		Clear();
	}

	FORCEINLINE void Clear()
	{
		FMemory::Memzero(&CurrentIndexBufferLocation, sizeof(CurrentIndexBufferLocation));
		FMemory::Memzero(&CurrentIndexBufferView, sizeof(CurrentIndexBufferView));
	}

	D3D12_INDEX_BUFFER_VIEW CurrentIndexBufferView;
	class FD3D12ResourceLocation* CurrentIndexBufferLocation;
};


struct FD3D12ConstantBufferCache : FD3D12ResourceCache<uint16>
{
	FD3D12ConstantBufferCache()
	{
		Clear();
	}

	void Clear()
	{
		FMemory::Memzero(CurrentGPUVirtualAddress, sizeof(CurrentGPUVirtualAddress));
	}

	D3D12_GPU_VIRTUAL_ADDRESS CurrentGPUVirtualAddress[ST_NumStandartTypes][16];

//	uint16 DirtySlotMask[ST_NumStandartTypes];
};



class FD3D12StateCache : public FD3D12DeviceChild
{
public:
	FD3D12StateCache(FD3D12CommandContext& CmdContext);

	void SetConstantBuffer(EShaderType ShaderType, FD3D12ConstantBuffer& Buffer, bool bDiscardConstants);


	void SetGrapicsPipelineState(class FD3D12GraphicsPipelineState* GraphicsPipelineState);
	void SetNewShaderData(EShaderType InType, const class FD3D12ShaderData* InShaderData);
	void SetStreamSource(FD3D12ResourceLocation* VertexBufferLocation, uint32 StreamIndex, uint32 Offset, uint32 Stride = 0);
	void SetIndexBuffer(FD3D12ResourceLocation* IndexBufferLocation, DXGI_FORMAT Format, uint32 Offset);
	void SetRenderTargets(uint32 NumRenderTargets, class FD3D12RenderTargetView** RTArray, class FD3D12DepthStencilView* DSTarget);
	void SetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ);

	void ApplyState(bool bIsCompute = false);

	void ApplyConstants(uint32 StartStage, uint32 EndStage, const class FD3D12RootSignature* RootSignature);



	void DirtyStateForNewCommandList();

	void QueueBindlessSRV(EShaderType Type, class FD3D12ShaderResourceView* SRV);
private:
	struct
	{
		struct
		{
			TRefCountPtr<class FD3D12GraphicsPipelineState> CurrentPipelineStateObject = nullptr;

			FD3D12RenderTargetView* RenderTargetArray[8];
			uint32 CurrentNumberRenderTargets = 0;
			FD3D12DepthStencilView* CurrentDepthStencilTarget;

			bool bNeedSetRootSignature = true;
			EPrimitiveType PrimitiveType;
			D3D_PRIMITIVE_TOPOLOGY PrimitiveTopology;
			FD3D12VertexBufferCache VBCache;
			FD3D12IndexBufferCache IBCache;
			uint32 PrimitiveTypeFactor;
			uint32 PrimitiveTypeOffset;

			uint32 CurrentNumberViewports = 0;
			D3D12_VIEWPORT CurrentViewport[16];

		} Graphics;

		struct
		{

		} Compute;

		struct
		{
			TRefCountPtr<ID3D12PipelineState> CurrentPipelineState;
			bool bNeedSetPSO;
			uint32 CurrentShaderSamplerCount[ST_NumStandartTypes] = {};
			uint32 CurrentShaderCBVCount[ST_NumStandartTypes] = {};
			uint32 CurrentShaderSRVCount[ST_NumStandartTypes] = {};
			uint32 CurrentShaderUAVCount[ST_NumStandartTypes] = {};

			FD3D12ConstantBufferCache CBVCache = {};
			TArray<FD3D12ShaderResourceView*> QueuedBindlessSRVs[ST_NumStandartTypes];
		} Common;
	} PipelineState;


	bool bNeedSetPrimitiveTopology = true;
	bool bNeedSetVB = true;
	bool bNeedSetRTs = true;
	bool bNeedSetViewports = true;
	bool bNeedSetScissorRect = true;
	bool bNeedSetBlendFactor = true;

public:
	uint32 GetVertexCount(uint32 NumPrimitives) const
	{
		return NumPrimitives * PipelineState.Graphics.PrimitiveTypeFactor + PipelineState.Graphics.PrimitiveTypeOffset;
	}

private:
	FD3D12CommandContext& Context;

};