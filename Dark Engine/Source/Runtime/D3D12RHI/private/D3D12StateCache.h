#pragma once
#include "D3D12RHICommon.h"
//#include "D3D12CommandContext.h"

class FD3D12CommandContext;


class FD3D12StateCache : public FD3D12DeviceChild
{
public:
	FD3D12StateCache(FD3D12CommandContext& CmdContext);



	void SetGrapicsPipelineState(class FD3D12GraphicsPipelineState* GraphicsPipelineState);
	void SetNewShaderData(EShaderType InType, const class FD3D12ShaderData* InShaderData);

private:
	struct
	{
		struct
		{
			TRefCountPtr<class FD3D12GraphicsPipelineState> CurrentPipelineStateObject = nullptr;
			bool bNeedSetRootSignature = true;
			EPrimitiveType PrimitiveType;
			D3D_PRIMITIVE_TOPOLOGY PrimitiveTopology;

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

private:
	FD3D12CommandContext& Context;

};