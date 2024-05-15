#include "SceneRendering.h"
#include "GlobalShader.h"
#include "CommonRenderResources.h"
#include "Timer/GameTimer.h"



class FFractalShaderVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FFractalShaderVS);
	SHADER_USE_PARAMETER_STRUCT(FFractalShaderVS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 0, 0, 0);
};


class FFractalShaderPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FFractalShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FFractalShaderPS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 1, 0, 0);
};

IMPLEMENT_GLOBAL_SHADER(FFractalShaderVS, "FractalShader.hlsl", "VSMain", ST_Vertex);
IMPLEMENT_GLOBAL_SHADER(FFractalShaderPS, "FractalShader.hlsl", "PSMain", ST_Pixel);


struct FFractalParams
{
	FVector2f Resolution;
	float Time;
};


void FSceneRender::RenderFractal(FRHICommandListImmediate& CmdList)
{
	TShaderRefBase<FFractalShaderVS> VertexShader = GGlobalShaderMap->GetShader<FFractalShaderVS>();
	TShaderRefBase<FFractalShaderPS> PixelShader = GGlobalShaderMap->GetShader<FFractalShaderPS>();

	TArray<uint8> Parameters;
	TArray<FRHIShaderParameterResource>	BindlessParameters;
	TArray<FRHIShaderParameterResource>	ResourceParameters;

	FFractalParams Params;
	Params.Time = FGameTimer::TotalTime();
	Params.Resolution = FVector2f(SceneView->RenderTarget->GetSizeXY().X, SceneView->RenderTarget->GetSizeXY().Y);
	static TRefCountPtr<FRHIUniformBuffer> UniformBuffer = RHICreateUniformBuffer(&Params, sizeof(Params), UniformBuffer_MultiFrame);
	RHIUpdateUniformBuffer(UniformBuffer.Get(), &Params, sizeof(Params));


	FRHIShaderParameterResource Param;
	Param.Type = FRHIShaderParameterResource::EType::UniformBuffer;
	Param.Resource = UniformBuffer.Get();
	Param.Index = 0;
	ResourceParameters.Add(Param);
	

	FRHIVertexShader* RHIVertexShader = VertexShader.GetVertexShader();
	FRHIPixelShader* RHIPixelShader = PixelShader.GetPixelShader();
	FRHIVertexDeclaration* RHIVertexDeclaration = GFilterVertexDeclaration->VertexDeclaration.Get();

	FGraphicsPipelineStateInitializer PSOInitializer = {};

	PSOInitializer.PrimitiveType = PT_TriangleList;
	PSOInitializer.BoundShaderState.VertexShaderRHI = RHIVertexShader;
	PSOInitializer.BoundShaderState.PixelShaderRHI = RHIPixelShader;
	PSOInitializer.BoundShaderState.VertexDeclaration = RHIVertexDeclaration;
	PSOInitializer.RenderTargetFormats[0] = EPixelFormat::PF_R8G8B8A8_UNORM;
	PSOInitializer.RasterizerState = RHICreateRasterizerState(FRasterizerStateInitializer());
	TRefCountPtr<FRHIGraphicsPipelineState>	PipelineState = RHICreateGraphicsPipelineState(PSOInitializer);

	//FRHIRenderPassInfo RenderPassInfo(SceneView->RenderTarget->GetRenderTargetTexture().Get());
	//CmdList.BeginRenderPass(RenderPassInfo);
	CmdList.SetGraphicsPipelineState(PipelineState.Get(), PSOInitializer.BoundShaderState);
	CmdList.SetShaderParameters(RHIPixelShader, Parameters, BindlessParameters, ResourceParameters);
	CmdList.RHISetViewport(SceneView->ViewRect.LeftUp.X, SceneView->ViewRect.LeftUp.Y,
		0.1f, SceneView->ViewRect.RightDown.X, SceneView->ViewRect.RightDown.Y, 100);
	CmdList.SetStreamSource(0, GRenctangleVertexBuffer->VertexBuffer.Get(),
		0, sizeof(FFilterVertex));

	CmdList.DrawIndexedPrimitive(GRenctangleIndexBuffer->IndexBuffer.Get(), 0, 0, 3, 0, 2, 1);

	//CmdList.EndRenderPass(RenderPassInfo);

}