#pragma once
#include "ShaderCore.h"
#include "IShaderFormat.h"


bool PreprocessD3DShader(
	const struct FShaderCompilerInput& Input,
	const struct FShaderCompilerEnvironment& MergedEvironment,
	class FShaderPreprocessOutput& PreprocessOutput
);


void CompileD3DShader
(
	const struct FShaderCompilerInput& Input,
	const class FShaderPreprocessOutput&,
	struct FShaderCompilerOutput& Output,
	const class FString& WorkingDirectory
);




class FD3DShaderFormat : public IShaderFormat
{
	virtual bool PreprocessShader(const FShaderCompilerInput& Input, const FShaderCompilerEnvironment& Environment, FShaderPreprocessOutput& PreprocessOutput) const override
	{
		return PreprocessD3DShader(Input, Environment, PreprocessOutput);
	}
	virtual void CompileShader(FString Name, const FShaderCompilerInput& Input, const FShaderPreprocessOutput& PreprocessOutput,
		FShaderCompilerOutput& Output) const
	{
		return CompileD3DShader(Input, PreprocessOutput, Output, TEXT(""));
	}


};