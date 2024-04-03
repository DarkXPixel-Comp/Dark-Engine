#include "Shader.h"
#include "HAL/DarkMemory.h"

TArray<const FShaderTypeRegistration*> GShaderTypeRegistationInstances;
TArray<FShaderType*> GShaderTypes;
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
