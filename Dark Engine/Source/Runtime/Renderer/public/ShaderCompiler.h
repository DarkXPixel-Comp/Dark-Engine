#pragma once
#include "Containers/DarkString.h"
#include "ShaderCore.h"
#include "GlobalShader.h"
#include "Memory/TUniquePtr.h"
#include "Containers/Array.h"


extern void CompileGlobalShaders();
extern void VerifyGlobalShaders();




class FShaderCompileJob
{
public:
	FShaderCompilerInput Input;
	FShaderPreprocessOutput	PreprocessOutput;
	FShaderCompilerOutput Output;
};


class FGlobalShaderTypeCompiler
{
public:
	static void BeginCompileShader(const FGlobalShaderType* ShaderType,
		TArray<TSharedPtr<FShaderCompileJob>>& Jobs);
	static FShader* FinishCompileShader(const FGlobalShaderType* ShaderType, const FShaderCompileJob& CompileJob);

	static void FinishCompilation(FString MaterialName, TArray<TSharedPtr<FShaderCompileJob>>& Jobs);

};