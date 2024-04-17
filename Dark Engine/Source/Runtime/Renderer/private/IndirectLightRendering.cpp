#include "GlobalShader.h"
#include "Misc/Paths.h"
#include "DynamicRHI.h"
#include <RHICommandList.h>


class FDiffuseIndirectCompositeVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FDiffuseIndirectCompositeVS)
	SHADER_USE_PARAMETER_STRUCT(FDiffuseIndirectCompositeVS, FGlobalShader);

	DECLARE_SHADER_BOUNDS(0, 2, 0, 0);
};


class FDiffuseIndirectCompositePS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FDiffuseIndirectCompositePS)
	SHADER_USE_PARAMETER_STRUCT(FDiffuseIndirectCompositePS, FGlobalShader);

	DECLARE_SHADER_BOUNDS(0, 3, 1, 0);

};


IMPLEMENT_GLOBAL_SHADER(FDiffuseIndirectCompositeVS, "VertexShader.hlsl",
	"main", ST_Vertex);
IMPLEMENT_GLOBAL_SHADER(FDiffuseIndirectCompositePS, "PixelShader.hlsl",
	"main", ST_Pixel);


void RenderLight()
{
	FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();

	TShaderRefBase<FDiffuseIndirectCompositeVS> VertexShader =
		GGlobalShaderMap->GetShader<FDiffuseIndirectCompositeVS>();
	TShaderRefBase<FDiffuseIndirectCompositePS>	PixelShader =
		GGlobalShaderMap->GetShader<FDiffuseIndirectCompositePS>();

	FVertexDeclarationElementList Elements;
	Elements.Add(FVertexElement(VET_Float3, 0, 0, 0, 0));
	Elements.Add(FVertexElement(VET_Float3, 1, 0, 0, 0));
	Elements.Add(FVertexElement(VET_Float4, 2, 0, 0, 0));
	Elements.Add(FVertexElement(VET_Float3, 3, 0, 0, 0));
	Elements.Add(FVertexElement(VET_Float3, 4, 0, 0, 0));
	Elements.Add(FVertexElement(VET_Float2, 5, 0, 0, 0));


	FRHIVertexShader* RHIVertexShader = VertexShader.GetVertexShader();
	FRHIPixelShader* RHIPixelShader = PixelShader.GetPixelShader();
	TRefCountPtr<FRHIVertexDeclaration> RHIVertexDeclaration = RHICreateVertexDeclaration(Elements);


	FGraphicsPipelineStateInitializer Initializer = {};
	Initializer.PrimitiveType = PT_TriangleList;
	Initializer.BoundShaderState.VertexShaderRHI = RHIVertexShader;
	Initializer.BoundShaderState.PixelShaderRHI = RHIPixelShader;
	Initializer.BoundShaderState.VertexDeclaration = RHIVertexDeclaration.Get();
	//RHICreateGraphicsPipelineState(Initializer);

	static TRefCountPtr<FRHIGraphicsPipelineState> PipelineState = RHICreateGraphicsPipelineState(Initializer);
	RHICmdList.SetGraphicsPipelineState(PipelineState.Get(), Initializer.BoundShaderState);




}