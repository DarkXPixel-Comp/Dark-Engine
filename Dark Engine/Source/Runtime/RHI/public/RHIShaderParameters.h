#pragma once
#include "RHIResources.h"

struct FRHIShaderParameter
{
	uint16 BufferIndex;
	uint16 BaseIndex;
	uint16 ByteOffset;
	uint16 ByteSize;
};

struct FRHIShaderParameterResource
{
	enum class EType
	{
		Texture,
		ResourceView,
		UAV,
		Sampler
	};

	FRHIResource* Resource = nullptr;
	uint16 Index = 0;
	EType Type = EType::Texture;
};


struct FRHIBatchedShaderParameters
{
	TArray<uint8> ParametersData;
	TArray<FRHIShaderParameter>	Parameters;
	TArray<FRHIShaderParameterResource>	ResourceParameters;
	TArray<FRHIShaderParameterResource>	BindlessParameters;
};