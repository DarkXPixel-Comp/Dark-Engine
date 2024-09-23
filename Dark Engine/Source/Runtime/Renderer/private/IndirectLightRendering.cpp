#include "GlobalShader.h"
#include "Misc/Paths.h"
#include "DynamicRHI.h"
#include <RHICommandList.h>
#include "CommonRenderResources.h"
#include "SceneRendering.h"
#include "GlobalResource.h"
#include "GameTimer.h"



class FScreenRectangleVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FScreenRectangleVS);
	SHADER_USE_PARAMETER_STRUCT(FScreenRectangleVS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 0, 0, 0);
};

class FScreenRectanglePS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FScreenRectanglePS);
	SHADER_USE_PARAMETER_STRUCT(FScreenRectanglePS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 2, 0, 0);
};


IMPLEMENT_GLOBAL_SHADER(FScreenRectangleVS, "ScreenRectangle.hlsl", "VSMain", ST_Vertex);
IMPLEMENT_GLOBAL_SHADER(FScreenRectanglePS, "ScreenRectangle.hlsl", "PSMain", ST_Pixel);


TGlobalRenderResource<FScreenRectangleVertexBuffer>	GRenctangleVertexBuffer;
TGlobalRenderResource<FScreenRectangleIndexBuffer> GRenctangleIndexBuffer;
TGlobalRenderResource<FFilterVertexDeclaration>	GFilterVertexDeclaration;


struct FQuadParam
{
	FVector2f NormalizeMousePosition;
	FIntPoint MousePosition;
	FIntPoint Resolution;
	float Time;
};

void FSceneRender::RenderQuad(FRHICommandListImmediate& RHICmdList)
{
	TShaderRefBase<FScreenRectangleVS> VertexShader =
		GGlobalShaderMap->GetShader<FScreenRectangleVS>();
	TShaderRefBase<FScreenRectanglePS>	PixelShader =
		GGlobalShaderMap->GetShader<FScreenRectanglePS>();


	TArray<uint8> Parameters;
	TArray<FRHIShaderParameterResource>	BindlessParameters;
	TArray<FRHIShaderParameterResource>	ResourceParameters;

	FQuadParam Params;
	Params.MousePosition = SceneView->ViewInitOptions.CursorPosition;
	Params.NormalizeMousePosition = SceneView->ViewInitOptions.NormalizeCursorPosition;
	Params.Resolution = SceneView->RenderTarget->GetSizeXY();
	Params.Time = FGameTimer::TotalTime();
	static TRefCountPtr<FRHIUniformBuffer> UniformBuffer = RHICreateUniformBuffer(&Params, sizeof(Params), UniformBuffer_MultiFrame);
	RHIUpdateUniformBuffer(UniformBuffer.Get(), &Params, sizeof(Params));

	FVector3f Vec3[2] = {FVector3f(0.5f, 0, 0), FVector3f(-0.5f, +0.2f, 0)};
	static TRefCountPtr<FRHIUniformBuffer> InstanceBuffer = RHICreateUniformBuffer(Vec3, sizeof(FVector3f) * 2, UniformBuffer_MultiFrame);
	RHIUpdateUniformBuffer(InstanceBuffer.Get(), Vec3, sizeof(FVector3f) * 2);

	FRHIShaderParameterResource Param;
	Param.Type = FRHIShaderParameterResource::EType::UniformBuffer;
	Param.Resource = UniformBuffer.Get();
	Param.Index = 1;
	ResourceParameters.Add(Param);



	//FVertexDeclarationElementList Elements;
	//Elements.Add(FVertexElement(VET_Float4, 0, 0, 0, 0));
	//Elements.Add(FVertexElement(VET_Float2, 1, 0, 0, 0));


	FRHIVertexShader* RHIVertexShader = VertexShader.GetVertexShader();
	FRHIPixelShader* RHIPixelShader = PixelShader.GetPixelShader();
	TRefCountPtr<FRHIVertexDeclaration> RHIVertexDeclaration = GFilterVertexDeclaration->VertexDeclaration;//RHICreateVertexDeclaration(Elements);

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


	/*FRHIRenderPassInfo RenderPassInfo(SceneView->RenderTarget->GetRenderTargetTexture().Get());
	RHICmdList.BeginRenderPass(RenderPassInfo);*/

	TRefCountPtr<FRHIGraphicsPipelineState> PipelineState = RHICreateGraphicsPipelineState(Initializer);
	RHICmdList.SetGraphicsPipelineState(PipelineState.Get(), Initializer.BoundShaderState);

	RHICmdList.RHISetViewport(SceneView->ViewRect.LeftUp.X, SceneView->ViewRect.LeftUp.Y, 0.1f, SceneView->ViewRect.RightDown.X, SceneView->ViewRect.RightDown.Y, 100);
	RHICmdList.SetStreamSource(0, GRenctangleVertexBuffer->VertexBuffer.Get(), 0, sizeof(FFilterVertex));
	RHICmdList.SetStreamSource(1, InstanceBuffer.Get(), 0, sizeof(FVector3f));
	RHICmdList.SetShaderParameters(RHIPixelShader, Parameters, BindlessParameters, ResourceParameters);
	RHICmdList.DrawIndexedPrimitive(GRenctangleIndexBuffer->IndexBuffer.Get(), 0, 0, 3, 6, 1, 2);


	/*RHICmdList.EndRenderPass(RenderPassInfo);*/

}