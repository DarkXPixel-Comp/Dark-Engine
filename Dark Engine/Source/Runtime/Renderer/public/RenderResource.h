#pragma once
#include "RHICommandList.h"
#include "RHIResources.h"



class FRenderResource
{
public:
	virtual void InitRHI(FRHICommandListImmediate& RHICmdList) {}

	virtual void ReleaseRHI() {}

	virtual void InitResource(FRHICommandListImmediate& RHICmdList) {}

	void UpdateRHI(FRHICommandList& RHICmdList) {}

};