#pragma once
#include "HAL/Platform.h"
#include "Containers/Array.h"
#include "Containers/DarkString.h"
#include "Containers/UnordoredMap.h"
#include "Memory/TUniquePtr.h"
#include "RHIDefines.h"
#include "RHIResources.h"

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

	class FParameters
	{
	public:
		virtual ~FParameters() {}
	};

	FShaderType
	(
		EShaderTypeInternal InShaderType,
		const TCHAR* InName,
		const TCHAR* InSourceFilename,
		const TCHAR* InFunctionName,
		uint32 InType
	);

	static TArray<const FShaderType*>& GetTypeList();
	static TUnordoredMap<FString, FShaderType*>& GetNameToTypeMap();

	class FGlobalShaderType* GetGlobalShaderType() const
	{
		return ShaderType == EShaderTypeInternal::Global ? (class FGlobalShaderType*)this : nullptr;
	}


	FORCEINLINE const TCHAR* GetName() const
	{
		return Name;
	}
	FORCEINLINE EShaderType GetShaderType() const
	{
		return Type;
	}
	FORCEINLINE const TCHAR* GetShaderFilename() const
	{
		return SourceFileName;
	}
	FORCEINLINE const TCHAR* GetFunctionName() const
	{
		return FunctionName;
	}




private:
	const TCHAR* Name;
	const TCHAR* SourceFileName;
	const TCHAR* FunctionName;
	EShaderTypeInternal ShaderType;
	EShaderType Type;
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
		//GetInstances().Add(this);
	}

	static TArray<const FShaderTypeRegistration*>& GetInstances();
	static void CommitAll();

private:
	std::function<FShaderType& ()> ShaderTypeAccessor;
};

struct FShaderCompiledInitializerType
{
	const FShaderType* Type;
	const FShaderType::FParameters* Parameters;
	const TArray<uint8>& Code;
	//const FShaderParameterMap
};



class FShaderMapResourceCode
{
public:
	struct FShaderEntry
	{
		TArray<uint8> Code;
		EShaderType ShaderType;
	};
};




class FShaderMapBase
{
public:
	FORCEINLINE FShaderMapResourceCode* GetResourceCode();


private:
	TSharedPtr<FShaderMapResourceCode> Code;


	
};


template<typename ContentType>
class TShaderMap : public FShaderMapBase
{
public:


};





template <typename ShaderType>
class TShaderRefBase
{
public:
	TShaderRefBase() : ShaderContent(nullptr) {}
	TShaderRefBase(ShaderType* InShader) : ShaderContent(InShader) {}

	
	FORCEINLINE ShaderType* GetShader() const
	{ 
		return ShaderContent;
	}

	FORCEINLINE FRHIShader* GetRHIShader() const
	{
		FRHIShader* RHIShader = nullptr;
		if (ShaderContent)
		{
			//RHIShader
		}
		return RHIShader;
	}

	FORCEINLINE FRHIVertexShader* GetVertexShader() const
	{
		return static_cast<FRHIVertexShader>(GetRHIShader());
	}



private:
	ShaderType* ShaderContent;

};



class FShader
{
public:
	using CompiledShaderInitializerType = FShaderCompiledInitializerType;


	FShader() {}

	FShader(const CompiledShaderInitializerType& Initializer) {}

	~FShader() {}



};










#define DECLARE_SHADER_TYPE(ShaderClass, InShaderMetaType, ...) \
public:\
using ShaderMetaType = F##InShaderMetaType##ShaderType;\
/*using ShaderMapType = F##InShaderMetaType##ShaerMap;\*/ \
static ShaderMetaType& GetStaticType();	\
private:\
static FShaderTypeRegistration ShaderTypeRegistration;\
public: static FShader* ConstructSerializedInstance() {return new ShaderClass();}\
static FShader* ConstructCompiledInstance(const typename FShader::CompiledShaderInitializerType& Initializer)\
{return new ShaderClass(static_cast<const typename ShaderMetaType::CompiledShaderInitializerType&>(Initializer));}\

#define IMPLEMENT_SHADER_TYPE(ShaderClass, SourceFilename, FunctionName, ShaderType)\
ShaderClass::ShaderMetaType& ShaderClass::GetStaticType()\
{\
	static ShaderClass::ShaderMetaType StaticType\
(\
	TEXT(#ShaderClass),	\
	SourceFilename,\
	FunctionName,\
	ShaderType\
);\
	return StaticType;\
}\
	FShaderTypeRegistration ShaderClass::ShaderTypeRegistration{std::function<FShaderType&()>{ShaderClass::GetStaticType}};\
\
\
\
\


#define SHADER_USE_PARAMETER_STRUCT(ShaderClass, ShaderParentClass) \
ShaderClass(const ShaderMetaType::CompiledShaderInitializerType& Initializer): ShaderParentClass(Initializer) {}\
ShaderClass() {}

