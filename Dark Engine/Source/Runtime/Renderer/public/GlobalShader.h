#pragma once
#include "Shader.h"
					  


class FGlobalShaderType : public FShaderType
{
public:
	typedef FShader::CompiledShaderInitializerType CompiledShaderInitializerType;

	FGlobalShaderType
	(
		const TCHAR* InName,
		const TCHAR* InSourceFilename,
		const TCHAR* InFunctionName,
		uint32 InType
	);



};


class FGlobalShaderMap
{
public:
	TShaderRefBase<FShader>	 GetShader(FShaderType* ShaderType) const;
	FORCEINLINE bool IsEmpty() const
	{
		return ShadersMap.empty();
	}



private:
	TUnordoredMap<FString, TShaderRefBase<FShader>>	ShadersMap;

};


class FGlobalShader : public FShader
{
public:
	using ShaderMetaType = FGlobalShaderType;

	FGlobalShader() : FShader() {}

	FGlobalShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FShader(Initializer) {}


};









#define DECLARE_GLOBAL_SHADER(ShaderClass) DECLARE_SHADER_TYPE(ShaderClass, Global)
#define IMPLEMENT_GLOBAL_SHADER(ShaderClass, SourceFilename, FunctionName, ShaderType) IMPLEMENT_SHADER_TYPE(ShaderClass, TEXT(SourceFilename), TEXT(FunctionName), ShaderType)

