#include "GlobalShader.h"

class FDiffuseIndirectCompositePS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FDiffuseIndirectCompositePS)
	SHADER_USE_PARAMETER_STRUCT(FDiffuseIndirectCompositePS, FGlobalShader);
};

IMPLEMENT_GLOBAL_SHADER(FDiffuseIndirectCompositePS, "test.shader", "Main", ST_Pixel);