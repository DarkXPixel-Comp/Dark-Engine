#include "RHICommandList.h"


FRHICommandListExecutor GRHICommandList;

FRHICommandListImmediate& FRHICommandListExecutor::GetImmediateCommandList()
{
	return GRHICommandList.CommandListImmediate;
}
