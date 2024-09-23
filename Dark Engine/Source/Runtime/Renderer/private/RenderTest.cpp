#include "SceneRendering.h"
#include "ScenePrivate.h"
#include "PrimitiveSceneInfo.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h" 
#include "GlobalShader.h"
#include "GlobalResource.h"


class FTestShaderVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FTestShaderVS);
	SHADER_USE_PARAMETER_STRUCT(FTestShaderVS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 0, 0, 0);
};
class FTestShaderPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FTestShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FTestShaderPS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 0, 0, 0);
};


IMPLEMENT_GLOBAL_SHADER(FTestShaderVS, "TestRender.hlsl", "VSMain", ST_Vertex);
IMPLEMENT_GLOBAL_SHADER(FTestShaderPS, "TestRender.hlsl", "PSMain", ST_Pixel);




class FTestVertexDeclaration : public FRenderResource
{
public:
	TRefCountPtr<FRHIVertexDeclaration>	VertexDeclaration;

	virtual void InitRHI(FRHICommandListImmediate& RHICmdList)
	{
		FVertexDeclarationElementList Elements;
		Elements.Add(FVertexElement(VET_Float3, 0, 0xff, 0, 0));
		VertexDeclaration = RHICreateVertexDeclaration(Elements);

	}
};


TGlobalRenderResource<FTestVertexDeclaration> GTestVertexDeclaration;


void FSceneRender::RenderTest(FRHICommandListImmediate& CmdList)
{
	for (auto& Primitive : Scene->Primitives)
	{
		auto Mesh = ((GStaticMeshComponent*)Primitive)->GetStaticMesh();
		auto RenderData = Mesh->GetRenderData();
		check(RenderData->IsInitialized());

		TShaderRefBase<FTestShaderVS> VertexShader = GGlobalShaderMap->GetShader<FTestShaderVS>();
		TShaderRefBase<FTestShaderPS> PixelShader = GGlobalShaderMap->GetShader<FTestShaderPS>();

		FRasterizerStateInitializer RasterState;
		RasterState.bAllowMSAA = false;
		RasterState.CullMode = RCM_None;
		RasterState.FillMode = RFM_Solid;

		FGraphicsPipelineStateInitializer Initializer = {};
		Initializer.PrimitiveType = PT_TriangleList;
		Initializer.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
		Initializer.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
		Initializer.BoundShaderState.VertexDeclaration = GTestVertexDeclaration->VertexDeclaration.Get();
		Initializer.RenderTargetFormats[0] = EPixelFormat::PF_R8G8B8A8_UNORM;
		Initializer.RasterizerState = RHICreateRasterizerState(RasterState);

		TRefCountPtr<FRHIGraphicsPipelineState>	PipelineState = RHICreateGraphicsPipelineState(Initializer);
		CmdList.SetGraphicsPipelineState(PipelineState.Get(), Initializer.BoundShaderState);
		CmdList.SetStreamSource(0, RenderData->LODResources[0]->VertexBuffers.PositionVertexBuffer.VertexBufferRHI.Get(), 0, RenderData->LODResources[0]->VertexBuffers.PositionVertexBuffer.VertexBufferRHI->GetStride());

		CmdList.DrawIndexedPrimitive(RenderData->LODResources[0]->IndexBuffer.IndexBufferRHI.Get(), 0, 0, RenderData->LODResources[0]->IndexBuffer.GetNumIndices(), 0, 1, 1);



	}

}