#include "ShaderCompiler.h"
#include "Shader.h"
#include "IShaderFormat.h"
#include "D3DShaderFormat.h"
#include "GlobalShader.h"
#include "ShaderCore.h"
#include "Timer/GameTimer.h"
#include "Containers/Array.h"
#include "Containers/DarkString.h"



FGlobalShaderMap* GGlobalShaderMap;
IShaderFormat* GShaderFormat = nullptr;;



void CompileGlobalShaders()
{
	if (!GGlobalShaderMap)
	{
		GGlobalShaderMap = new FGlobalShaderMap();
	}
	if (!GShaderFormat)
	{
		GShaderFormat = new FD3DShaderFormat();//TEMP;
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
		FGlobalShaderTypeCompiler::FinishCompilation(TEXT("Global"), GlobalShaderJobs);
	}
}

void FGlobalShaderTypeCompiler::BeginCompileShader(const FGlobalShaderType* ShaderType,
	TArray<TSharedPtr<FShaderCompileJob>>& Jobs)
{
	FShaderCompileJob* NewJob = new FShaderCompileJob();
	NewJob->Input.EntryPoint = ShaderType->GetFunctionName();
	NewJob->Input.SourceFilePath = ShaderType->GetShaderFilename();
	NewJob->Input.ShaderType = ShaderType->GetShaderType();
	NewJob->Input.Type = (FShaderType*)ShaderType;
	Jobs.Add(MakeShareble(NewJob));
}


static bool PreprocessShaderInternal(const IShaderFormat* Compiler, FShaderCompileJob& Job)
{
	const float StartPreprocessTime = FGameTimer::GameTime();

	bool bSuccess = Compiler->PreprocessShader(Job.Input, Job.Input.Environment, Job.PreprocessOutput);


	Job.PreprocessOutput.PreprocessTime = FGameTimer::GameTime() - StartPreprocessTime;
	return bSuccess;
}

void CompileShader(FShaderCompileJob& Job)
{
	const IShaderFormat* Compiler = GShaderFormat;
	if (!Compiler)
	{
		DE_LOG(LogShaders, Fatal, TEXT("Not find shader compiler"));
	}

	float TimeStart = FGameTimer::GameTime();
	
	//PreprocessShaderInternal(Compiler, Job);


	if (PreprocessShaderInternal(Compiler, Job))
	{
		Compiler->CompileShader(TEXT("Global"), Job.Input, Job.PreprocessOutput, Job.Output);
	}

}


void ProcessCompiledShaderMap(TArray<TSharedPtr<FShaderCompileJob>>& Jobs)
{
	for (auto& It : Jobs)
	{
		GGlobalShaderMap->FindOrAddShader((FGlobalShaderType*)It->Input.Type,
			It->Input.Type->FinishCompileShader(*It, TEXT("Global")));
	}
}



void FGlobalShaderTypeCompiler::FinishCompilation(FString MaterialName, TArray<TSharedPtr<FShaderCompileJob>>& Jobs)
{
	for (auto& It : Jobs)
	{
		CompileShader(*It);
	}

	ProcessCompiledShaderMap(Jobs);

}
