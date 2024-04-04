#include "GlobalShader.h"

FGlobalShaderType::FGlobalShaderType(
	const TCHAR* InName,
	const TCHAR* InSourceFilename, 
	const TCHAR* InFunctionName,
	uint32 InType
):
	FShaderType(EShaderTypeInternal::Global,InName, InSourceFilename, InFunctionName, InType)
{

}

TShaderRefBase<FShader> FGlobalShaderMap::GetShader(FShaderType* ShaderType) const
{
	auto ShaderIt = ShadersMap.find(ShaderType->GetName());
	return ShaderIt != ShadersMap.end() ? ShaderIt->second : TShaderRefBase<FShader>();
}
