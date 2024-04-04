#pragma once
#include "Containers/DarkString.h"


class IShaderFormat
{
public:
	virtual void CompileShader(FString Format, const struct FShaderCompilerInput& Input) = 0;

};