#include "D3D12RootSignature.h"
#include "RHIResources.h"
#include "D3D12Adapter.h"
#include "D3D12Util.h"
#include "D3D12Shader.h"


FD3D12RootSignature* FD3D12RootSignatureManager::GetRootSignature(const FBoundShaderStateInput& BSS)
{
	auto It = RootSignatureMap.find(BSS);

	if (It == RootSignatureMap.end())
	{
		return CreateRootSignature(BSS);
	}

	return It->second;
}

FD3D12RootSignature* FD3D12RootSignatureManager::CreateRootSignature(const FBoundShaderStateInput& BSS)
{
	FD3D12RootSignature* NewRootSignature = new FD3D12RootSignature(GetParentAdapter(), BSS);

	RootSignatureMap.emplace(BSS, NewRootSignature);
	return NewRootSignature;
}

void FD3D12RootSignature::Init(const FBoundShaderStateInput& BSS)
{
	FD3D12RootSignatureDesc Desc(BSS);

	Init(Desc.GetDesc());
	Hash = Desc.Hash;
}

void FD3D12RootSignature::Init(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& InDesc)
{
	ID3D12Device* Device = GetParentAdapter()->GetD3DDevice();

	TRefCountPtr<ID3DBlob> Error;


	const D3D_ROOT_SIGNATURE_VERSION MaxRootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	DXCall(D3DX12SerializeVersionedRootSignature(&InDesc, MaxRootSignatureVersion,
		&RootSignatureBlob, &Error));


	if (Error)
	{
		FString StringError = (ANSICHAR*)Error->GetBufferPointer();
		DE_LOG(D3D12RHI, Fatal, TEXT("Error create root signature: %s"), *StringError);
		return;
	}
	DXCall(Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&RootSignature)));

	AnalyzeSinature(InDesc.Desc_1_1, 0);
}

void FD3D12RootSignature::AnalyzeSinature(const D3D12_ROOT_SIGNATURE_DESC1& Desc, uint32 BindingSpace)
{
	FMemory::Memzero(BindSlotMap, sizeof(BindSlotMap));


	for (uint32 i = 0; i < Desc.NumParameters; ++i)
	{
		const auto& CurrentParameter = Desc.pParameters[i];

		EShaderType CurrentShaderType = ST_NumStandartTypes;

		switch (CurrentParameter.ShaderVisibility)
		{
		case D3D12_SHADER_VISIBILITY_ALL:
			CurrentShaderType = ST_NumTypes;
			break;
		case D3D12_SHADER_VISIBILITY_VERTEX:
			CurrentShaderType = ST_Vertex;
			break;
		case D3D12_SHADER_VISIBILITY_HULL:
			break;
		case D3D12_SHADER_VISIBILITY_DOMAIN:
			break;
		case D3D12_SHADER_VISIBILITY_GEOMETRY:
			CurrentShaderType = ST_Geometry;
			break;
		case D3D12_SHADER_VISIBILITY_PIXEL:	
			CurrentShaderType = ST_Pixel;
			break;
		case D3D12_SHADER_VISIBILITY_AMPLIFICATION:
			break;
		case D3D12_SHADER_VISIBILITY_MESH:
			break;
		default:
			break;
		}

		switch (CurrentParameter.ParameterType)
		{
		case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
		{
			const auto& CurrentRange = CurrentParameter.DescriptorTable.pDescriptorRanges[0];
			switch (CurrentRange.RangeType)
			{
			case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
				SetSRVDTBindSlot(CurrentShaderType, i);
				break;
			case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
				SetCBVDTBindSlot(CurrentShaderType, i);
				break;
			default:
				break;
			}
			break;
		}

		case D3D12_ROOT_PARAMETER_TYPE_CBV:
			if (CurrentParameter.Descriptor.ShaderRegister == 0)
			{
				SetCBVDBindSlot(CurrentShaderType, i);
			}

			break;
		default:
			break;
		}
	}



}


/**
* @todo Rewrite THIS!!
*/
FD3D12RootSignatureDesc::FD3D12RootSignatureDesc(const FBoundShaderStateInput& BSS)
{
	uint32 RootParameterCount = 0;

	FD3D12VertexShader* VertexShader = (FD3D12VertexShader*)BSS.VertexShaderRHI;
	if (VertexShader)
	{
		if (VertexShader->ResourceCounts.NumSRVs > 0)
		{
			DescriptorRanges[RootParameterCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, VertexShader->ResourceCounts.NumSRVs, 0, 0);
			TableSlots[RootParameterCount].InitAsDescriptorTable(1, &DescriptorRanges[RootParameterCount], D3D12_SHADER_VISIBILITY_VERTEX);
			++RootParameterCount;
			RootParameterSize += 1;
			hash_combine(Hash, VertexShader->ResourceCounts.NumSRVs);
		}
		if (VertexShader->ResourceCounts.NumCBVs > 0)
		{
			for (uint32 ShaderRegister = 0; (ShaderRegister < VertexShader->ResourceCounts.NumCBVs) && (ShaderRegister < 16); ++ShaderRegister)
			{
				TableSlots[RootParameterCount].InitAsConstantBufferView(ShaderRegister, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);
				++RootParameterCount;
				RootParameterSize += 2;
			}
			if (VertexShader->ResourceCounts.NumCBVs > 16)
			{
				DescriptorRanges[RootParameterCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, VertexShader->ResourceCounts.NumCBVs - 16, 16, 0);
				TableSlots[RootParameterCount].InitAsDescriptorTable(1, &DescriptorRanges[RootParameterCount], D3D12_SHADER_VISIBILITY_VERTEX);
				++RootParameterCount;
				RootParameterSize += 1;
				hash_combine(Hash, VertexShader->ResourceCounts.NumCBVs);
			}
		}
		if (VertexShader->ResourceCounts.NumSamplers > 0)
		{
			DescriptorRanges[RootParameterCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, VertexShader->ResourceCounts.NumSamplers, 0, 0);
			TableSlots[RootParameterCount].InitAsDescriptorTable(1, &DescriptorRanges[RootParameterCount], D3D12_SHADER_VISIBILITY_VERTEX);
			++RootParameterCount;
			RootParameterSize += 1;
			hash_combine(Hash, VertexShader->ResourceCounts.NumSamplers);
		}
	}


	FD3D12PixelShader* PixelShader = (FD3D12PixelShader*)BSS.PixelShaderRHI;
	if (PixelShader)
	{
		if (PixelShader->ResourceCounts.NumSRVs > 0)
		{
			DescriptorRanges[RootParameterCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, PixelShader->ResourceCounts.NumSRVs, 0, 0);
			TableSlots[RootParameterCount].InitAsDescriptorTable(1, &DescriptorRanges[RootParameterCount], D3D12_SHADER_VISIBILITY_PIXEL);
			++RootParameterCount;
			RootParameterSize += 1;
			hash_combine(Hash, PixelShader->ResourceCounts.NumSRVs);
		}
		if (PixelShader->ResourceCounts.NumCBVs > 0)
		{
			for (uint32 ShaderRegister = 0; (ShaderRegister < PixelShader->ResourceCounts.NumCBVs) && (ShaderRegister < 16); ++ShaderRegister)
			{
				TableSlots[RootParameterCount].InitAsConstantBufferView(ShaderRegister, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_PIXEL);
				++RootParameterCount;
				RootParameterSize += 2;
			}
			if (PixelShader->ResourceCounts.NumCBVs > 16)
			{
				DescriptorRanges[RootParameterCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, PixelShader->ResourceCounts.NumCBVs - 16, 16, 0);
				TableSlots[RootParameterCount].InitAsDescriptorTable(1, &DescriptorRanges[RootParameterCount], D3D12_SHADER_VISIBILITY_PIXEL);
				++RootParameterCount;
				RootParameterSize += 1;
				hash_combine(Hash, PixelShader->ResourceCounts.NumCBVs);
			}
		}
		if (PixelShader->ResourceCounts.NumSamplers > 0)
		{
			DescriptorRanges[RootParameterCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, PixelShader->ResourceCounts.NumSamplers, 0, 0);
			TableSlots[RootParameterCount].InitAsDescriptorTable(1, &DescriptorRanges[RootParameterCount], D3D12_SHADER_VISIBILITY_PIXEL);
			++RootParameterCount;
			RootParameterSize += 1;
			hash_combine(Hash, PixelShader->ResourceCounts.NumSamplers);
		}
	}


	CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100);

	static const CD3DX12_STATIC_SAMPLER_DESC StaticSamplers[] =
	{
		CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100),
		CD3DX12_STATIC_SAMPLER_DESC(1, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100),
		CD3DX12_STATIC_SAMPLER_DESC(2, D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100),
		CD3DX12_STATIC_SAMPLER_DESC(3, D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100),
		CD3DX12_STATIC_SAMPLER_DESC(4, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100),
		CD3DX12_STATIC_SAMPLER_DESC(5, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100),
		CD3DX12_STATIC_SAMPLER_DESC(6, D3D12_FILTER_COMPARISON_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100),
		CD3DX12_STATIC_SAMPLER_DESC(7, D3D12_FILTER_COMPARISON_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0.0f, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, DE_MAX_FLT, D3D12_SHADER_VISIBILITY_ALL, 100)
	};


	RootDesc.Init_1_1(RootParameterCount, TableSlots, DE_ARRAY_COUNT(StaticSamplers), StaticSamplers, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED | D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED);

}