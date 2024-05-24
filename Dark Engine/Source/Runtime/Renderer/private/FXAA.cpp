#include "GlobalShader.h"
#include "Misc/Paths.h"
#include "DynamicRHI.h"
#include <RHICommandList.h>
#include "CommonRenderResources.h"
#include "SceneRendering.h"
#include "GlobalResource.h"


class FFXAAShaderCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FFXAAShaderCS);
	SHADER_USE_PARAMETER_STRUCT(FFXAAShaderCS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 2, 0, 0);
};



IMPLEMENT_GLOBAL_SHADER(FFXAAShaderCS, "FXAA.hlsl", "FXAAMain", ST_Compute);


void FSceneRender::FXAA(FRHICommandListImmediate& CmdList)
{
	TShaderRefBase<FFXAAShaderCS> FXAAShader = GGlobalShaderMap->GetShader<FFXAAShaderCS>();
	TRefCountPtr<FRHITexture> BackBufferTexture = SceneView->RenderTarget->GetRenderTargetTexture();

	FIntPoint Resolution = BackBufferTexture->GetSize();

	static TRefCountPtr<FRHIUniformBuffer> UniformBuffer =
		RHICreateUniformBuffer(&Resolution, sizeof(Resolution), UniformBuffer_MultiFrame);
	RHIUpdateUniformBuffer(UniformBuffer.Get(), &Resolution, sizeof(Resolution));


	TArray<uint8> Parameters;
	TArray<FRHIShaderParameterResource>	BindlessParameters;
	TArray<FRHIShaderParameterResource>	ResourceParameters;

	FRHIShaderParameterResource InputTexture;
	InputTexture.Index = 0;
	InputTexture.Type = FRHIShaderParameterResource::EType::Texture;
	InputTexture.Resource = BackBufferTexture.Get();
	BindlessParameters.Add(InputTexture);
	
	FRHIShaderParameterResource OutputTexture;
	OutputTexture.Index = 1;
	OutputTexture.Type = FRHIShaderParameterResource::EType::UAV;
	OutputTexture.Resource = SceneTextures.GBufferA->GetUnorderedAccessView();
	BindlessParameters.Add(OutputTexture);

	FRHIShaderParameterResource ParamResolution;
	ParamResolution.Index = 1;
	ParamResolution.Type = FRHIShaderParameterResource::EType::UniformBuffer;
	ParamResolution.Resource = UniformBuffer.Get();
	ResourceParameters.Add(ParamResolution);

	

	FRHIComputeShader* RHIFXAAShader = FXAAShader.GetComputeShader();

	TRefCountPtr<FRHIComputePipelineState> FXAAPipeline = RHICreateComputePipelineState(RHIFXAAShader);
	CmdList.SetComputePipelineState(FXAAPipeline.Get());
	CmdList.SetShaderParameters(RHIFXAAShader, Parameters, BindlessParameters, ResourceParameters);
	CmdList.DispatchComputeShader(Resolution.X / 32U + 1, Resolution.Y / 32U + 1, 1);

	//RenderTextureQuad(CmdList, SceneTextures.GBufferA);

	CmdList.CopyTexture(SceneTextures.GBufferA.Get(), BackBufferTexture.Get());
	
}