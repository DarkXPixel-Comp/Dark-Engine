#pragma once
#include "Shader.h"
					  


class FGlobalShaderType : public FShaderType
{
public:


};


class FGlobalShader : public FShader
{
public:
	using ShaderMetaType = FGlobalShaderType;


};









#define DECLARE_GLOBAL_SHADER(ShaderClass) DECLARE_SHADER_TYPE(ShaderClass, Global)
#define IMPLEMENT_GLOBAL_SHADER(ShaderClass, SourceFilename, FunctionName, ShaderType) IMPLEMENT_SHADER_TYPE(ShaderClass, TEXT(SourceFilename), TEXT(FunctionName), ShaderType)

