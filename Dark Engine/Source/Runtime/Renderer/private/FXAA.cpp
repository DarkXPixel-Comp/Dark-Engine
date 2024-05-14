#include "GlobalShader.h"
#include "Misc/Paths.h"
#include "DynamicRHI.h"
#include <RHICommandList.h>
#include "CommonRenderResources.h"
#include "SceneRendering.h"
#include "GlobalResource.h"


class FFXAAShaderCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FFXAAShaderCS);
	SHADER_USE_PARAMETER_STRUCT(FFXAAShaderCS, FGlobalShader);
	DECLARE_SHADER_BOUNDS(0, 1, 0, 0);
};



IMPLEMENT_GLOBAL_SHADER(FFXAAShaderCS, "FXAA.hlsl", "Main", ST_Compute);


