#pragma once
#include "Containers/DarkString.h"


class IShaderFormat
{
public:
	//virtual void CompileShader(FString Format, const struct FShaderCompilerInput& Input) = 0;
	virtual bool PreprocessShader(const FShaderCompilerInput& Input, const FShaderCompilerEnvironment& Environment,
		FShaderPreprocessOutput& Output) const { return false; }
	virtual void CompileShader(FString Name, const FShaderCompilerInput& Input, const FShaderPreprocessOutput& PreprocessOutput,
		FShaderCompilerOutput& Output) const {}

};