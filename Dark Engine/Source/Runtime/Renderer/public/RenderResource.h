#pragma once
#include "RHICommandList.h"
#include "RHIResources.h"



class FRenderResource
{
public:
	virtual void InitRHI(FRHICommandListImmediate& RHICmdList) {}

	virtual void ReleaseRHI() {}

	virtual void InitResource(FRHICommandListImmediate& RHICmdList) {}

	void UpdateRHI(FRHICommandListImmediate& RHICmdList)
	{
		ReleaseRHI();
		InitRHI(RHICmdList);
	}

};

class FVertexBuffer : public FRenderResource
{
public:
	virtual void ReleaseRHI() override;

	TRefCountPtr<FRHIBuffer> VertexBufferRHI;
};

class FIndexBuffer : public FRenderResource
{
public:
	TRefCountPtr<FRHIBuffer> IndexBufferRHI;
};