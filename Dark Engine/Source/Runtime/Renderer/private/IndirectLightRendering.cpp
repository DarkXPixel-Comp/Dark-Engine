#include "GlobalShader.h"
#include "Misc/Paths.h"


class FDiffuseIndirectCompositePS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FDiffuseIndirectCompositePS)
	SHADER_USE_PARAMETER_STRUCT(FDiffuseIndirectCompositePS, FGlobalShader);

	DECLARE_SHADER_BOUNDS(0, 2, 0, 0);

	int k = 5;
};


IMPLEMENT_GLOBAL_SHADER(FDiffuseIndirectCompositePS, "VertexShader.hlsl",
	"main", ST_Vertex);


void RenderLight()
{
	TShaderRefBase<FDiffuseIndirectCompositePS> VertexShader =
		GGlobalShaderMap->GetShader<FDiffuseIndirectCompositePS>();
	FRHIVertexShader* RHIVertexShader = VertexShader.GetVertexShader();
}