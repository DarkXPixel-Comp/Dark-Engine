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
	auto ShaderIt = ShadersMap.find(ShaderType->GetStringName());
	return ShaderIt != ShadersMap.end() ? ShaderIt->second : TShaderRefBase<FShader>();
}


FShader* FGlobalShaderMap::FindOrAddShader(FGlobalShaderType* ShaderType, FShader* InShader)
{
	auto ShaderIt = ShadersMap.find(ShaderType->GetStringName());
	if (ShaderIt == ShadersMap.end())
	{
		InShader->SetShaderBounds(ShaderType->Bounds);
		ShadersMap.emplace(ShaderType->GetStringName(), TShaderRefBase<FShader>(InShader));
		return InShader;
	}
	return ShaderIt->second.GetShader();
}