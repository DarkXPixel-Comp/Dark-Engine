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


	TArray<TSharedPtr<FShaderCompileJob>> GlobalShaderJobs;

	for (auto& ShaderTypeIt : FShaderType::GetTypeList())
	{
		FGlobalShaderType* GlobalShaderType = ShaderTypeIt->GetGlobalShaderType();
		if (!GlobalShaderType)
		{
			continue;
		}
		FGlobalShaderTypeCompiler::BeginCompileShader(GlobalShaderType, GlobalShaderJobs);
	}

	if (GlobalShaderJobs.Num() > 0)
	{

	}

}

void FGlobalShaderTypeCompiler::BeginCompileShader(const FGlobalShaderType* ShaderType,
	TArray<TSharedPtr<FShaderCompileJob>>& Jobs)
{
	FShaderCompileJob* NewJob = new FShaderCompileJob();
	Jobs.Add(MakeShareble(NewJob));
}
