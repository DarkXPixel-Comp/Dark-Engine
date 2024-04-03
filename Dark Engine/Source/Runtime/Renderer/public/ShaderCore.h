#pragma once
#include "Containers/UnordoredMap.h"
#include "Containers/DarkString.h"

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