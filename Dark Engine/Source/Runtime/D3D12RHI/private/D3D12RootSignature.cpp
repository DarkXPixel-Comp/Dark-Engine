#include "D3D12RootSignature.h"
#include "RHIResources.h"
#include "D3D12Adapter.h"
#include "d3dx12.h"
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

	DXCall(Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&RootSignature)));
}

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
			DescriptorRanges[RootParameterCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, VertexShader->ResourceCounts.NumCBVs, 0, 0);
			TableSlots[RootParameterCount].InitAsDescriptorTable(1, &DescriptorRanges[RootParameterCount], D3D12_SHADER_VISIBILITY_VERTEX);
			++RootParameterCount;
			RootParameterSize += 1;
			hash_combine(Hash, VertexShader->ResourceCounts.NumCBVs);
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


	FD3D12VertexShader* PixelShader = (FD3D12VertexShader*)BSS.PixelShaderRHI;
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
			DescriptorRanges[RootParameterCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, PixelShader->ResourceCounts.NumCBVs, 0, 0);
			TableSlots[RootParameterCount].InitAsDescriptorTable(1, &DescriptorRanges[RootParameterCount], D3D12_SHADER_VISIBILITY_PIXEL);
			++RootParameterCount;
			RootParameterSize += 1;
			hash_combine(Hash, PixelShader->ResourceCounts.NumCBVs);
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


	RootDesc.Init_1_1(RootParameterCount, TableSlots, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

}
