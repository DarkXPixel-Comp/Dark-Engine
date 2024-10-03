#include "ShaderCompiler.h"
#include "Shader.h"
#include "IShaderFormat.h"
#include "D3DShaderFormat.h"
#include "GlobalShader.h"
#include "ShaderCore.h"
#include "Timer/GameTimer.h"
#include "Containers/Array.h"
#include "Containers/DarkString.h"
#include "BS_thread_pool.hpp"
#include <future>



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
	else
	{
		DE_LOG(LogShaders, Error, TEXT("GlobalShaderJobs.Num() = 0"));
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
	DE_LOG(LogShaders, Log, TEXT("Compile %s"), ShaderType->GetShaderFilename());
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
		return;
	}

	DE_LOG(LogShaders, Log, TEXT("Finish compile %s"), *Job.Input.SourceFilePath);
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
	if (ASYNC_SHADER_COMPILING)
	{
		DE_LOG(LogShaders, Log, TEXT("Begin Async shader compiling"));
		BS::thread_pool ThreadPool(std::thread::hardware_concurrency() / 2);

		for (auto& It : Jobs)
		{
			ThreadPool.submit_task([&It]() { CompileShader(*It); });
		}

		ThreadPool.wait();
	}
	else
	{
		DE_LOG(LogShaders, Log, TEXT("Begin default shader compiling"));
		for (auto& It : Jobs)
		{
			DE_LOG(LogShaders, Log, TEXT("Default shader"));
			CompileShader(*It);
		}

	}
	
	ProcessCompiledShaderMap(Jobs);

}
