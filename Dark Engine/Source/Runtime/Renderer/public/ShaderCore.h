#pragma once
#include "Containers/UnordoredMap.h"
#include "Containers/DarkString.h"
#include "Logging/LogMacros.h"

#include <variant>

DECLARE_LOG_CATEGORY(LogShaders, Display)


class FShaderCompilerDefinitions
{
public:
	FShaderCompilerDefinitions() = default;

	void SetDefine(const TCHAR* Name, const TCHAR* Value)
	{
		Definitions.emplace(Name, Value);
	}

	void SetDefine(const TCHAR* Name, const FString& Value)
	{
		Definitions.emplace(Name, Value);
	}

	void SetDefine(const TCHAR* Name, bool Value)
	{
		Definitions.emplace(Name, Value ? TEXT("1") : TEXT("0"));
	}
	void SetDefine(const TCHAR* Name, uint32 Value)
	{
		Definitions.emplace(Name, FString::PrintF(TEXT("%u"), Value));
	}
	void SetDefine(const TCHAR* Name, int32 Value)
	{
		Definitions.emplace(Name, FString::PrintF(TEXT("%i"), Value));
	}
	void SetDefine(const TCHAR* Name, float Value)
	{
		Definitions.emplace(Name, FString::PrintF(TEXT("%f"), Value));
	}
	const TMap<FString, FString>& GetDefinitionMap() const
	{
		return Definitions;
	}

private:
	TMap<FString, FString> Definitions;
};

struct FShaderCompilerEnvironment
{
	FShaderCompilerDefinitions Definitions;
	TMap<FString, std::variant<bool, float, int32, uint32, FString>> CompileArgs;

};




struct FShaderCompilerInput
{
	FString	ShaderFormat;
	FString SourceFilePath;
	FString EntryPoint;
	FString ShaderName;
	uint32 ShaderType;
	class FShaderType* Type;

	FShaderCompilerEnvironment Environment;
};

struct FShaderPreprocessOutput
{
	FString PreprocessedSource;
	FString OriginalPreprocessedSource;
	float PreprocessTime;
};

struct FShaderCode
{
	TArray<uint8> ShaderCodeWithOptionalData;
};


struct FShaderCompilerOutput
{
	FShaderCode ShaderCode;
	std::size_t ShaderHash = 0;
};


struct FShaderBounds
{
	uint8 SamplerCount;
	uint8 ConstantBufferCount;
	uint8 ShaderResourceCount;
	uint8 UnorderedAccessCount;
};