#pragma once
#include "RHIResources.h"
#include <d3d12.h>
#include "Shader.h"


struct FShaderCodeResourceCounts
{
	FShaderCodeResourceCounts(const FShaderBounds& InBounds) : 
		NumSamplers(InBounds.SamplerCount),
		NumSRVs(InBounds.ShaderResourceCount),
		NumCBVs(InBounds.ConstantBufferCount),
		NumUAVs(InBounds.UnorderedAccessCount)
	{}
	FShaderCodeResourceCounts() {}

	uint8 NumSamplers;
	uint8 NumSRVs;
	uint8 NumCBVs;
	uint8 NumUAVs;
};

struct FD3D12ShaderData
{
	TArray<uint8> Code;
	FShaderCodeResourceCounts ResourceCounts{};

	D3D12_SHADER_BYTECODE GetShaderByteCode() const
	{
		return CD3DX12_SHADER_BYTECODE(Code.GetData(), Code.Num());
	}
};



template <typename TShaderType>
TShaderType* CreateStandartShader(const TArray<uint8>& InCode)
{
	TShaderType* Shader = new TShaderType();
	Shader->Code = InCode;
	Shader->ShaderHash = std::hash<TArray<uint8>>{}(InCode);


	return Shader;

}

typedef TArray<D3D12_INPUT_ELEMENT_DESC> FD3D12VertexElements;

class FD3D12VertexDeclaration : public FRHIVertexDeclaration
{
public:
	FD3D12VertexElements VertexElements;
	FD3D12VertexDeclaration(const FD3D12VertexElements& InElements):
		VertexElements(InElements)
	{}


	D3D12_INPUT_LAYOUT_DESC GetLayoutDsec() const
	{
		return D3D12_INPUT_LAYOUT_DESC{VertexElements.GetData(), (UINT)VertexElements.Num()};
	}

	FString SemanticName;



};

class FD3D12VertexShader : public FRHIVertexShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Vertex };

};


class FD3D12PixelShader : public FRHIPixelShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Pixel };

};

class FD3D12GeometryShader : public FRHIGeometryShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Geometry };

};

class FD3D12ComputeShader : public FRHIComputeShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Compute };

};