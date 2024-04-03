#pragma once
#include "HAL/Platform.h"
#include "Containers/Array.h"
#include "Containers/DarkString.h"
#include "RHIDefines.h"

#include <functional>






class FShaderType
{
public:
	enum class EShaderTypeInternal
	{
		Global,
		Material,
		MeshMaterial,
		NumShaderTypes
	};


private:
	const TCHAR* Name;
	const TCHAR* SourceFileName;
	const TCHAR* FunctionName;
	EShaderTypeInternal ShaderType;
};


class FShaderPipelineType
{
public:
	FShaderPipelineType
	(
		const TCHAR* InName,
		const FShaderType* InVertexShader,
		const FShaderType* InGeometryShader,
		const FShaderType* InPixelShader
		);

	static TArray<const FShaderPipelineType*>& GetTypeList();

	static void Initialize();


protected:
	const TCHAR* const Name;
	const FString TypeName;

	TArray<const FShaderType*> Stages;
	const FShaderType* AllStages[ST_NumStandartTypes];

};


class FShaderTypeRegistration
{
public:
	FShaderTypeRegistration(std::function<FShaderType& ()> InShaderTypeAccessor):
		ShaderTypeAccessor(InShaderTypeAccessor)
	{
		GetInstances().Add(this);
	}

	static TArray<const FShaderTypeRegistration*>& GetInstances();
	static void CommitAll();

private:
	std::function<FShaderType& ()> ShaderTypeAccessor;
};


class FShader
{
	FShader();
	~FShader();



};










#define DECLARE_SHADER_TYPE(ShaderClass, ShaderMetaType, ...) \
public:\
using ShaderMetaType = F##ShaderMetaType##ShaderType;\
using ShaderMapType = F##ShaderMetaType##ShaerMap;\
static FShader* ConstructSerializedInstance() {return new ShaderClass();}\
static FShader* ConstructCompiledInstance(const typename FShader::CompiledShaderInitializerType& Initializer)\
{return new ShaderClass(static_cast<const typename ShaderMetaType::CompiledShaderInitializerType&>(Initializer));}\

#define IMPLEMENT_SHADER_TYPE(ShaderClass, SourceFilename, FunctionName, ShaderType)\
ShaderClass::ShaderMetaType& ShaderClass::GetStaticType()\
{\
	static ShaderClass::ShaderMetaType StaticType\
(\
	Shader\
)\
}\