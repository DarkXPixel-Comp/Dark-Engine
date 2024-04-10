#include "PipelineStateCache.h"
#include "Templates/RefCounting.h"
#include "RHIResources.h"
#include "RHICommandList.h"
#include <type_traits>


class FPipelineState
{
public:

};


class FGraphicsPipelineState
{
public:
	TRefCountPtr<FRHIGraphicsPipelineState>	RHIPipeline;
};




TUnordoredMap<FGraphicsPipelineStateInitializer, FGraphicsPipelineState*> FPipelineStateCache::GGraphicsPipelineMap;

FGraphicsPipelineStateInitializer* FPipelineStateCache::GetAndOrCreateGraphicsPipelineState(FRHICommandList& RHICmdList, const FGraphicsPipelineStateInitializer& Initializer)
{
	FGraphicsPipelineState* Result = nullptr;
	bool bWasFound = GGraphicsPipelineMap.find(Initializer) != GGraphicsPipelineMap.end();

	if (!bWasFound)
	{
		Result = new FGraphicsPipelineState();



		GGraphicsPipelineMap.emplace(Initializer, Result);
	}



	return nullptr;
}


