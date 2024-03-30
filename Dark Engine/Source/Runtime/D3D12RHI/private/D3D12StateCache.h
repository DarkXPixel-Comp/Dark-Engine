#pragma once
#include "D3D12RHICommon.h"
#include "D3D12CommandContext.h"


class FD3D12StateCache : public FD3D12DeviceChild
{
public:
	FD3D12StateCache(FD3D12CommandContext& CmdContext);



private:
	struct
	{
		struct
		{

		} Graphics;

		struct
		{

		} Compute;

		struct
		{
			TRefCountPtr<ID3D12PipelineState> CurrentPipelineState;
			bool bNeedSetPSO;




		} Common;
	} PipelineState;






private:
	FD3D12CommandContext& Context;

};