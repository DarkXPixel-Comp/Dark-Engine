#pragma once
#include "Containers/DarkString.h"


class IShaderFormat
{
public:
	virtual void CompileShader(FString Format, const class FShaderCompilerInput& Input);

};