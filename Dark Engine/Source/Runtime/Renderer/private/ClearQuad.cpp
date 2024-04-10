#include "GlobalShader.h"

class FOneColorVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FOneColorVS)
	SHADER_USE_PARAMETER_STRUCT(FOneColorVS, FGlobalShader);
};

IMPLEMENT_GLOBAL_SHADER(FOneColorVS, "", "", ST_Vertex);