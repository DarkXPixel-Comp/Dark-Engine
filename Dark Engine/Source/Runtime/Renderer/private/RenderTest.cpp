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
	DECLARE_SHADER_BOUNDS(0, 2, 0, 0);
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
	struct FCBV1
	{
		FMatrix ViewMatrix;
		FMatrix ProjectionMatrix;
		FMatrix ViewProjectionMatrix;
	} CBV1;		  //<-- Test

	if (Scene->Camera)
	{
		CBV1.ViewMatrix = Scene->Camera->CameraMatrices.ViewMatrix;
		CBV1.ProjectionMatrix = Scene->Camera->CameraMatrices.ProjectionMatrix;
		CBV1.ViewProjectionMatrix = Scene->Camera->CameraMatrices.ViewProjectionMatrix;
	}

	static TRefCountPtr<FRHIUniformBuffer> UniformBuffer = RHICreateUniformBuffer(&CBV1, sizeof(FCBV1), UniformBuffer_SingleFrame);
	RHIUpdateUniformBuffer(UniformBuffer.Get(), &CBV1, sizeof(FCBV1));

	for (auto& Primitive : Scene->Primitives)
	{
		auto Mesh = ((GStaticMeshComponent*)Primitive)->GetStaticMesh();
		auto RenderData = Mesh->GetRenderData();
		check(RenderData->IsInitialized());

		TArray<uint8> Parameters;
		TArray<FRHIShaderParameterResource>	BindlessParameters;
		TArray<FRHIShaderParameterResource>	ResourceParameters;

		FRHIShaderParameterResource Param0;
		Param0.Type = FRHIShaderParameterResource::EType::UniformBuffer;
		Param0.Resource = UniformBuffer.Get();
		Param0.Index = 0;
		ResourceParameters.Add(Param0);

		FRHIShaderParameterResource Param1;
		Param1.Type = FRHIShaderParameterResource::EType::UniformBuffer;
		Param1.Resource = Primitive->RHIRenderMatrix.Get();
		Param1.Index = 1;
		ResourceParameters.Add(Param1);


		TShaderRefBase<FTestShaderVS> VertexShader = GGlobalShaderMap->GetShader<FTestShaderVS>();
		TShaderRefBase<FTestShaderPS> PixelShader = GGlobalShaderMap->GetShader<FTestShaderPS>();

		FRasterizerStateInitializer RasterState;
		RasterState.bAllowMSAA = false;
		RasterState.CullMode = RCM_Forward;
		RasterState.FillMode = RFM_Solid;

		FGraphicsPipelineStateInitializer Initializer = {};
		Initializer.PrimitiveType = PT_TriangleList;
		Initializer.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
		Initializer.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
		Initializer.BoundShaderState.VertexDeclaration = GTestVertexDeclaration->VertexDeclaration.Get();
		Initializer.RenderTargetFormats[0] = EPixelFormat::PF_R8G8B8A8_UNORM;
		Initializer.RasterizerState = RHICreateRasterizerState(RasterState);

		TRefCountPtr<FRHIGraphicsPipelineState>	PipelineState = RHICreateGraphicsPipelineState(Initializer);
		CmdList.RHISetViewport(SceneView->ViewRect.LeftUp.X, SceneView->ViewRect.LeftUp.Y,
			0.1f, SceneView->ViewRect.RightDown.X, SceneView->ViewRect.RightDown.Y, 100);
		CmdList.SetGraphicsPipelineState(PipelineState.Get(), Initializer.BoundShaderState);
		CmdList.SetStreamSource(0, RenderData->LODResources[0]->VertexBuffers.PositionVertexBuffer.VertexBufferRHI.Get(), 0, RenderData->LODResources[0]->VertexBuffers.PositionVertexBuffer.VertexBufferRHI->GetStride());
		CmdList.SetShaderParameters(VertexShader.GetVertexShader(), Parameters, BindlessParameters, ResourceParameters);
		CmdList.DrawIndexedPrimitive(RenderData->LODResources[0]->IndexBuffer.IndexBufferRHI.Get()
			, 0
			, 0
			, RenderData->LODResources[0]->VertexBuffers.PositionVertexBuffer.VertexBufferRHI->GetSize()
			, 0
			, RenderData->LODResources[0]->IndexBuffer.GetNumIndices() / 3
			, 1);



	}

}