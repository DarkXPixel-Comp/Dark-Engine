#include "RHICommandList.h"
#include "DynamicRHI.h"
#include "imgui.h"
#include <Logger.h>


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

	Logger::log("[D3D12RHI]InitDefaultContexts");


}