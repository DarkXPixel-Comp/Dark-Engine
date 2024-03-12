#pragma once
#include "D3D12RHICommon.h"



struct FD3D12PipelineState : public  FD3D12AdapterChild
{

	TRefCountPtr<ID3D12PipelineState> PSO;
};



struct FD3D12GraphicsPipelineState
{

};