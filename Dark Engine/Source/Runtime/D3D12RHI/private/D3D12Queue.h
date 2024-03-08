#pragma once

#include "D3D12RHICommon.h"
#include "RHIPipeline.h"


enum class ED3D12QueueType
{
	Direct = 0,
	Copy = 1,
	Async = 2,
	Count = 3
};



FORCEINLINE ED3D12QueueType GetD3DCommandQueueType(ERHIPipeline Pipeline)
{
	switch (Pipeline)
	{
	case ERHIPipeline::Graphics: return ED3D12QueueType::Direct;
	case ERHIPipeline::AsyncCompute: return ED3D12QueueType::Async;
	}
}


FORCEINLINE const TCHAR* GetD3D12CommandQueueTypeName(ED3D12QueueType QueueType)
{
	switch (QueueType)
	{
	case ED3D12QueueType::Direct: return TEXT("Direct");
	case ED3D12QueueType::Copy:	return TEXT("Copy");
	case ED3D12QueueType::Async: return TEXT("Async");
	}
}

FORCEINLINE D3D12_COMMAND_LIST_TYPE GetD3DCommandListType(ED3D12QueueType QueueType)
{
	switch (QueueType)
	{
	case ED3D12QueueType::Direct: return D3D12_COMMAND_LIST_TYPE_DIRECT;
	case ED3D12QueueType::Copy:	return D3D12_COMMAND_LIST_TYPE_COPY;
	case ED3D12QueueType::Async: return D3D12_COMMAND_LIST_TYPE_COMPUTE;
	}
}
