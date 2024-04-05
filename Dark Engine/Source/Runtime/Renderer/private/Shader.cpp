#include "Shader.h"
#include "HAL/DarkMemory.h"

TArray<const FShaderTypeRegistration*> GShaderTypeRegistationInstances;
TArray<const FShaderType*> GShaderTypes;
TUnordoredMap<FString, FShaderType*> GNameToTypeMap;
TArray<const FShaderPipelineType*> GShaderPipelineTypes;



TArray<const FShaderTypeRegistration*>& FShaderTypeRegistration::GetInstances()
{
	return GShaderTypeRegistationInstances;
}

void FShaderTypeRegistration::CommitAll()
{
	for (const auto* Instance : GetInstances())
	{
		FShaderType& ShaderType = Instance->ShaderTypeAccessor();
	}
	GetInstances().Empty();
}

FShaderPipelineType::FShaderPipelineType(const TCHAR* InName, const FShaderType* InVertexShader, const FShaderType* InGeometryShader, const FShaderType* InPixelShader):
	Name(InName),
	TypeName(InName)
{
	FMemory::Memzero(AllStages, sizeof(AllStages));

	if (InPixelShader)
	{
		Stages.Add(InPixelShader);
		AllStages[ST_Pixel] = InPixelShader;
	}
	if (InGeometryShader)
	{
		Stages.Add(InGeometryShader);
		AllStages[ST_Geometry] = InGeometryShader;
	}
	if(InVertexShader)
	{
		Stages.Add(InVertexShader);
		AllStages[ST_Vertex] = InVertexShader;
	}

	GetTypeList().Add(this);



}

TArray<const FShaderPipelineType*>& FShaderPipelineType::GetTypeList()
{
	return GShaderPipelineTypes;
}

FShaderType::FShaderType
(
	EShaderTypeInternal InShaderType,
	const TCHAR* InName, 
	const TCHAR* InSourceFilename, 
	const TCHAR* InFunctionName, 
	uint32 InType
):
	ShaderType(InShaderType),
	Name(InName),
	SourceFileName(InSourceFilename),
	FunctionName(InFunctionName),
	Type((EShaderType)InType)
{
	GetTypeList().Add(this);
	GetNameToTypeMap().emplace(Name, this);
}

TArray<const FShaderType*>& FShaderType::GetTypeList()
{
	return GShaderTypes;
}

TUnordoredMap<FString, FShaderType*>& FShaderType::GetNameToTypeMap()
{
	return GNameToTypeMap;
}