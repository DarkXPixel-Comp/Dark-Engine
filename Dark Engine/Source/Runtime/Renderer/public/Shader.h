#pragma once


class FShader
{
	FShader();
	~FShader();



};









#define DECLARE_SHADER_TYPE(ShaderClass, ShaderMetaType, ...) \
public:\
using ShaderMetaType = F##ShaderMetaType;\
static FShader* ConstructSerializedInstance() {return new ShaderClass();}\
static FShader* ConstructCompiledInstance(const typename FShader::CompiledShaderInitializerType& Initializer)\
{return new ShaderClass(static_cast<const typename ShaderMetaType::CompiledShaderInitializerType&>(Initializer));}\
\
\
\