#pragma once
#include "RHIResources.h"
#include "Containers/UnordoredMap.h"

class FPipelineStateCache
{
public:
	static FGraphicsPipelineStateInitializer* GetAndOrCreateGraphicsPipelineState(
		class FRHICommandList& RHICmdList,
		const FGraphicsPipelineStateInitializer& Initializer);

private:
	static TUnordoredMap<FGraphicsPipelineStateInitializer, class FGraphicsPipelineState*> GGraphicsPipelineMap;
};