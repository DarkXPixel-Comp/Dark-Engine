#pragma once
#include "D3D12RHICommon.h"
//#include "D3D12CommandContext.h"

class FD3D12CommandContext;


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
	}


	D3D12_VERTEX_BUFFER_VIEW CurrentVertexBufferViews[32];
	class FD3D12ResourceLocation* CurrentVertexBufferResources[32];

};



class FD3D12StateCache : public FD3D12DeviceChild
{
public:
	FD3D12StateCache(FD3D12CommandContext& CmdContext);



	void SetGrapicsPipelineState(class FD3D12GraphicsPipelineState* GraphicsPipelineState);
	void SetNewShaderData(EShaderType InType, const class FD3D12ShaderData* InShaderData);
	void SetStreamSource(FD3D12ResourceLocation* VertexBufferLocation, uint32 StreamIndex, uint32 Offset, uint32 Stride = 0);
	void SetIndexBuffer(FD3D12ResourceLocation* IndexBufferLocation, DXGI_FORMAT Format, uint32 Offset);

private:
	struct
	{
		struct
		{
			TRefCountPtr<class FD3D12GraphicsPipelineState> CurrentPipelineStateObject = nullptr;
			bool bNeedSetRootSignature = true;
			EPrimitiveType PrimitiveType;
			D3D_PRIMITIVE_TOPOLOGY PrimitiveTopology;
			FD3D12VertexBufferCache VBCache;
			uint32 PrimitiveTypeFactor;
			uint32 PrimitiveTypeOffset;

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
			




		} Common;
	} PipelineState;


	bool bNeedSetPrimitiveTopology = true;
	bool bNeedSetSetVB = true;



public:
	uint32 GetVertexCount(uint32 NumPrimitives) const
	{
		return NumPrimitives * PipelineState.Graphics.PrimitiveTypeFactor + PipelineState.Graphics.PrimitiveTypeOffset;
	}

private:
	FD3D12CommandContext& Context;

};