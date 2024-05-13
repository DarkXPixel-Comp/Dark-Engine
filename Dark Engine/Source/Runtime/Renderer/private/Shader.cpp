#include "Shader.h"
#include "HAL/DarkMemory.h"
#include <ShaderCompiler.h>
#include "DynamicRHI.h"

TArray<const FShaderTypeRegistration*> GShaderTypeRegistationInstances = TArray<const FShaderTypeRegistration*>();
TArray<const FShaderType*> GShaderTypes;
TUnordoredMap<FString, FShaderType*> GNameToTypeMap;
TArray<const FShaderPipelineType*> GShaderPipelineTypes;


FShader::FShader(const FShaderCompiledInitializerType& Initializer):
	Type(Initializer.Type),
	Code(Initializer.Code)
{

}

TRefCountPtr<FRHIShader> FShader::GetRHIShader()
{
	if (!RHIShader)
	{
		switch (Type->GetShaderType())
		{
		case ST_Vertex:
			RHIShader = RHICreateVertexShader(Code, Bounds);
			break;
		case ST_Mesh:
			break;
		case ST_Pixel:
			RHIShader = RHICreatePixelShader(Code, Bounds);
			break;
		case ST_Geometry:
			break;
		case ST_Compute:
			RHIShader = RHICreateComputeShader(Code, Bounds);
			break;
		case ST_RayGen:
			break;
		}
	}

	return RHIShader;
}


FShader* FShaderType::FinishCompileShader(FShaderCompileJob& CurrentJob, const FString& InDebugDescription)
{
	FShader* Shader = new FShader(FShaderCompiledInitializerType({this, nullptr, 
		CurrentJob.Output.ShaderCode.ShaderCodeWithOptionalData}));
	Shader->HashShader = CurrentJob.Output.ShaderHash;

	return Shader;
}

FShaderTypeRegistration::FShaderTypeRegistration(std::function<FShaderType& ()> InShaderTypeAccessor) :
	ShaderTypeAccessor(InShaderTypeAccessor)
{
	GShaderTypeRegistationInstances.Add(this);
}


TArray<const FShaderTypeRegistration*>& FShaderTypeRegistration::GetInstances()
{
	return GShaderTypeRegistationInstances;
}

void FShaderTypeRegistration::CommitAll()
{
	for (const auto* Instance : GShaderTypeRegistationInstances)
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


