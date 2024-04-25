#include "GlobalShader.h"
#include "Misc/Paths.h"
#include "DynamicRHI.h"
#include <RHICommandList.h>
#include "CommonRenderResources.h"
#include "SceneRendering.h"
#include "GlobalResource.h"



class FScreenRectangleVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FScreenRectangleVS);
	SHADER_USE_PARAMETER_STRUCT(FScreenRectangleVS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 1, 0, 0);
};

class FScreenRectanglePS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FScreenRectanglePS);
	SHADER_USE_PARAMETER_STRUCT(FScreenRectanglePS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 1, 0, 0);
};


IMPLEMENT_GLOBAL_SHADER(FScreenRectangleVS, "ScreenRectangle.hlsl", "VSMain", ST_Vertex);
IMPLEMENT_GLOBAL_SHADER(FScreenRectanglePS, "ScreenRectangle.hlsl", "PSMain", ST_Pixel);


TGlobalRenderResource<FScreenRectangleVertexBuffer>	GRenctangleVertexBuffer;
TGlobalRenderResource<FScreenRectangleIndexBuffer> GRenctangleIndexBuffer;
TGlobalRenderResource<FFilterVertexDeclaration>	GFilterVertexDeclaration;



void FSceneRender::RenderQuad(FRHICommandListImmediate& RHICmdList)
{
	TShaderRefBase<FScreenRectangleVS> VertexShader =
		GGlobalShaderMap->GetShader<FScreenRectangleVS>();
	TShaderRefBase<FScreenRectanglePS>	PixelShader =
		GGlobalShaderMap->GetShader<FScreenRectanglePS>();

	FVertexDeclarationElementList Elements;
	Elements.Add(FVertexElement(VET_Float4, 0, 0, 0, 0));
	Elements.Add(FVertexElement(VET_Float2, 1, 0, 0, 0));

	//Elements[0].Name = "SV_POSITION";
	//Elements[1].Name = "TEXCOORD";


	FRHIVertexShader* RHIVertexShader = VertexShader.GetVertexShader();
	FRHIPixelShader* RHIPixelShader = PixelShader.GetPixelShader();
	TRefCountPtr<FRHIVertexDeclaration> RHIVertexDeclaration = RHICreateVertexDeclaration(Elements);

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

	TRefCountPtr<FRHIGraphicsPipelineState> PipelineState = RHICreateGraphicsPipelineState(Initializer);
	RHICmdList.SetGraphicsPipelineState(PipelineState.Get(), Initializer.BoundShaderState);


	RHICmdList.RHISetViewport(SceneView->ViewRect.LeftUp.X, SceneView->ViewRect.LeftUp.Y, 0.1f, SceneView->ViewRect.RightDown.X, SceneView->ViewRect.RightDown.Y, 100);
	RHICmdList.SetStreamSource(0, GRenctangleVertexBuffer->VertexBuffer.Get(), 0, sizeof(FFilterVertex));
	RHICmdList.DrawIndexedPrimitive(GRenctangleIndexBuffer->IndexBuffer.Get(), 0, 0, 3, 0, 2, 1);

}