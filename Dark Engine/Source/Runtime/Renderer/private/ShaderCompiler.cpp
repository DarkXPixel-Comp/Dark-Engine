#include "ShaderCompiler.h"
#include "Shader.h"
#include "IShaderFormat.h"
#include "D3DShaderFormat.h"
#include "GlobalShader.h"
#include "ShaderCore.h"



FGlobalShaderMap* GGlobalShaderMap;



void CompileGlobalShaders()
{
	if (!GGlobalShaderMap)
	{
		GGlobalShaderMap = new FGlobalShaderMap();
	}

	VerifyGlobalShaders();
}


void VerifyGlobalShaders()
{
	check(GGlobalShaderMap);

	if (GGlobalShaderMap->IsEmpty())
	{
		DE_LOG(LogShaders, Log, TEXT("Recompiling all global shaders"));
	}


	for (auto& ShaderTypeIt : FShaderType::GetTypeList())
	{
		FGlobalShaderType* GlobalShaderType = ShaderTypeIt->GetGlobalShaderType();
		if (!GlobalShaderType)
		{
			continue;
		}
		TShaderRefBase<FShader>	GlobalShader = GGlobalShaderMap->GetShader(GlobalShaderType);
	//	FGlobalShaderTypeCompiler::BeginCompileShader(GlobalShader);


	}


}