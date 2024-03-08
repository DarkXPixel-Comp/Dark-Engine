#include "RHICommandList.h"
#include "RHIResources.h"




class FRenderResource
{
public:
	virtual void InitRHI(FRHICommandListBase& RHICmdList) {}

	virtual void ReleaseRHI() {}

	virtual void InitResource(FRHICommandListBase& RHICmdList) {}

	void UpdateRHI(FRHICommandList& RHICmdList) {}

	template <typename T>
	FRHIBuffer CreateRHIBuffer(T& InOutResourceObject, const uint32 ResourceCount, const TCHAR* InDebugName);






};