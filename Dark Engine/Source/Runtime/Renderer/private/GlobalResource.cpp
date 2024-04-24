#include "GlobalResource.h"
#include "RHICommandList.h"

TArray<FRenderResource*> InitializeArray;


TArray<FRenderResource*>& GetInitializingRenderResources()
{
	return InitializeArray;
}

void InitGlobalRenderResources()
{
	FRHICommandListImmediate& CmdList = FRHICommandListExecutor::GetImmediateCommandList();

	for (auto& Resource : TGlobalRenderResource<uint8>::GetInitArray())
	{
		Resource->InitRHI(CmdList);
	}
}