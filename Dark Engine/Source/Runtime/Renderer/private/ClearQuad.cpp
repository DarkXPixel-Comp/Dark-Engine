#include "GlobalShader.h"
#include "SceneRendering.h"
#include "CommonRenderResources.h"

class FClearQuadVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FClearQuadVS)
	SHADER_USE_PARAMETER_STRUCT(FClearQuadVS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 0, 0, 0);
};

class FOneTexturePS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FOneTexturePS)
	SHADER_USE_PARAMETER_STRUCT(FOneTexturePS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 1, 0, 0);

};

IMPLEMENT_GLOBAL_SHADER(FClearQuadVS, "ClearQuad.hlsl", "ClearQuadVS", ST_Vertex);
IMPLEMENT_GLOBAL_SHADER(FOneTexturePS, "ClearQuad.hlsl", "OneTexturePS", ST_Pixel);



void FSceneRender::RenderTextureQuad(FRHICommandListImmediate& CmdList, TRefCountPtr<FRHITexture> InTexture)
{
	TShaderRefBase<FClearQuadVS> VertexShader =
		GGlobalShaderMap->GetShader<FClearQuadVS>();
	TShaderRefBase<FOneTexturePS> PixelShader =
		GGlobalShaderMap->GetShader<FOneTexturePS>();

	FRHIVertexShader* RHIVertexShader = VertexShader.GetVertexShader();
	FRHIPixelShader* RHIPixelShader = PixelShader.GetPixelShader();
	TRefCountPtr<FRHIVertexDeclaration> RHIVertexDeclaration = GFilterVertexDeclaration->VertexDeclaration;

	TArray<uint8> Parameters;
	TArray<FRHIShaderParameterResource>	BindlessParameters;
	TArray<FRHIShaderParameterResource>	ResourceParameters;

	FRHIShaderParameterResource BindlessTexture;
	BindlessTexture.Index = 0;
	BindlessTexture.Resource = InTexture.Get();
	BindlessTexture.Type = FRHIShaderParameterResource::EType::Texture;
	BindlessParameters.Add(BindlessTexture);


	FRasterizerStateInitializer RasterState;
	RasterState.bAllowMSAA = false;
	RasterState.CullMode = RCM_None;
	RasterState.FillMode = RFM_Solid;

	FGraphicsPipelineStateInitializer Initializer = {};
	Initializer.PrimitiveType = PT_TriangleList;
	Initializer.BoundShaderState.VertexShaderRHI = RHIVertexShader;
	Initializer.BoundShaderState.PixelShaderRHI = RHIPixelShader;
	Initializer.BoundShaderState.VertexDeclaration = RHIVertexDeclaration.Get();
	Initializer.RenderTargetFormats[0] = EPixelFormat::PF_R8G8B8A8_UNORM;
	Initializer.RasterizerState = RHICreateRasterizerState(RasterState);

	FRHIRenderPassInfo RenderPassInfo(SceneView->RenderTarget->GetRenderTargetTexture().Get());
	CmdList.BeginRenderPass(RenderPassInfo);

	TRefCountPtr<FRHIGraphicsPipelineState>	PipelineState = RHICreateGraphicsPipelineState(Initializer);
	CmdList.SetGraphicsPipelineState(PipelineState.Get(), Initializer.BoundShaderState);
	CmdList.RHISetViewport(SceneView->ViewRect.LeftUp.X, SceneView->ViewRect.LeftUp.Y, 0.1f, SceneView->ViewRect.RightDown.X, SceneView->ViewRect.RightDown.Y, 100);
	CmdList.SetStreamSource(0, GRenctangleVertexBuffer->VertexBuffer.Get(), 0, sizeof(FFilterVertex));
	CmdList.SetShaderParameters(RHIPixelShader, Parameters, BindlessParameters, ResourceParameters);
	CmdList.DrawIndexedPrimitive(GRenctangleIndexBuffer->IndexBuffer.Get(), 0, 0, 3, 0, 2, 1);

	CmdList.EndRenderPass(RenderPassInfo);
}
