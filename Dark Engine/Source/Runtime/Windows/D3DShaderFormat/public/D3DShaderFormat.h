#pragma once
#include "ShaderCore.h"


bool PreprocessD3DShader(
	const struct FShaderCompilerInput& Input,
	const struct FShaderCompilerEnvironment& MergedEvironment,
	class FShaderPreprocessOutput& PreprocessOutput
);


void CompileD3DShader
(
	const struct FShaderCompilerInput& Input,
	const class FShaderPreprocessorOutput&,
	const class FString& WorkingDirectory
);




class FD3DShaderFormat : 