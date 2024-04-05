#include "GlobalShader.h"
#include "Misc/Paths.h"


class FDiffuseIndirectCompositePS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FDiffuseIndirectCompositePS)
	SHADER_USE_PARAMETER_STRUCT(FDiffuseIndirectCompositePS, FGlobalShader);
};


IMPLEMENT_GLOBAL_SHADER(FDiffuseIndirectCompositePS, "VertexShader.hlsl",
	"main", ST_Vertex);


void RenderLight()
{

}