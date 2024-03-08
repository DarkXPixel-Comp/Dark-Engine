#include "RHICommandList.h"
#include "DynamicRHI.h"
#include "imgui.h"


FRHICommandListExecutor GRHICommandList;

FRHICommandListImmediate& FRHICommandListExecutor::GetImmediateCommandList()
{
	return GRHICommandList.CommandListImmediate;
}


void FRHICommandListImmediate::BeginFrame()
{
	GetContext().RHIBeginFrame();
}


void FRHICommandListImmediate::InitializeContexts()
{
	if (GDynamicRHI)
	{
		SwitchPipeline();
	}


}