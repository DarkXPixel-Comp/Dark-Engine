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
	DECLARE_SHADER_BOUNDS(0, 0, 0, 0);
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

	FRHITextureCreateDesc Test(TEXT("TEST"), ETextureDimension::Texture2D);
	Test.Extent = FIntPoint(32, 32);
	Test.SetFormat(EPixelFormat::PF_B8G8R8A8_UNORM);


	static TRefCountPtr<FRHITexture> Texture = RHICreateTexture(Test);

	static bool Test1 = true;

	uint32 Stride;
	uint64 LockSize;
	if (Test1)
	{
		void* Data = RHILockTexture2D(Texture.Get(), 0, RLM_WriteOnly, Stride, &LockSize);
		TArray<uint8> RandArray(LockSize);

		for (uint64 i = 0; i < LockSize; i++)
		{
			RandArray[i] = FMath::RandRange(0, 0xFF);
		}

		FMemory::Memcpy(Data, RandArray.GetData(), LockSize);
	/*	FMath::RandRange(0, 0xFF);
		FMemory::Memset(Data, FMath::RandRange(0, 0xFF), LockSize);*/

		RHIUnlockTexture2D(Texture.Get(), 0);
		Test1 = false;
	}


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


	TArray<uint8> Parameters;
	TArray<FRHIShaderParameterResource>	BindlessParameters;
	TArray<FRHIShaderParameterResource>	ResourceParameters;

	FRHIShaderParameterResource BindlessTexture;
	BindlessTexture.Resource = Texture.Get();

	BindlessParameters.Add(BindlessTexture);
	BindlessTexture.Index = 1;
	BindlessParameters.Add(BindlessTexture);

	//FRHIRenderTargetView RenderTarget(SceneView->RenderTarget->GetRenderTargetTexture().Get());

	FRHIRenderPassInfo RenderPassInfo(SceneView->RenderTarget->GetRenderTargetTexture().Get());
	RHICmdList.BeginRenderPass(RenderPassInfo);

	TRefCountPtr<FRHIGraphicsPipelineState> PipelineState = RHICreateGraphicsPipelineState(Initializer);
	RHICmdList.SetGraphicsPipelineState(PipelineState.Get(), Initializer.BoundShaderState);
	RHICmdList.SetShaderParameters(RHIPixelShader, Parameters, BindlessParameters, ResourceParameters);

	RHICmdList.RHISetViewport(SceneView->ViewRect.LeftUp.X, SceneView->ViewRect.LeftUp.Y, 0.1f, SceneView->ViewRect.RightDown.X, SceneView->ViewRect.RightDown.Y, 100);
	RHICmdList.SetStreamSource(0, GRenctangleVertexBuffer->VertexBuffer.Get(), 0, sizeof(FFilterVertex));
	RHICmdList.DrawIndexedPrimitive(GRenctangleIndexBuffer->IndexBuffer.Get(), 0, 0, 3, 0, 2, 1);

	RHICmdList.EndRenderPass(RenderPassInfo);

}