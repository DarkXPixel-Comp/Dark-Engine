#include "RHIResources.h"

FRHISetRenderTargetInfo FRHIRenderPassInfo::ConvertToRenderTargetInfo() const
{
	FRHISetRenderTargetInfo Result;

	for (int32 i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		if (ColorRenderTargets[i].RenderTarget == nullptr)
		{
			Result.ColorRenderTarget[i].Texture = nullptr;
			break;
		}
		Result.ColorRenderTarget[i].Texture = ColorRenderTargets[i].RenderTarget;
		Result.ColorRenderTarget[i].MipIndex = ColorRenderTargets[i].MipIndex;
		Result.ColorRenderTarget[i].ArraySlice = ColorRenderTargets[i].ArraySlice;
		Result.BeginRenderPassMode[i] = ColorRenderTargets[i].RenderPassBeginMode;
		Result.EndRenderPassMode[i] = ColorRenderTargets[i].RenderPassModeEnd;
		++Result.NumColorRenderTargets;
	}

	Result.DepthStencilRenderTarget = FRHIDepthRenderTargetView();
	return Result;
}