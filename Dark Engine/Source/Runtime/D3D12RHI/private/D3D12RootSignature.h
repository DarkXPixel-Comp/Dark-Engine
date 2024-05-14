#pragma once

#include "D3D12RHICommon.h"
#include "Containers/UnordoredMap.h"
#include "Containers/Array.h"




enum ERootParameterKeys
{
	PS_SRVs,
	PS_CBVs,
	PS_RootCBVs,
	PS_Samplers,
	VS_SRVs,
	VS_CBVs,
	VS_RootCBVs,
	VS_Samplers,

	CS_RootCBVs,
	CS_CBVs,



	RPK_Count
};



class FD3D12RootSignatureDesc
{
public:
	explicit FD3D12RootSignatureDesc(const FBoundShaderStateInput& BSS);
	explicit FD3D12RootSignatureDesc(const class FD3D12ComputeShader* ComputeShader);

	static constexpr uint32 MaxRootParameters = 32;

	FORCEINLINE const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& GetDesc() const { return RootDesc; }

	uint64 Hash = 0;
private:
	uint32 RootParameterSize = 0;
	CD3DX12_ROOT_PARAMETER1 TableSlots[MaxRootParameters];
	CD3DX12_DESCRIPTOR_RANGE1 DescriptorRanges[MaxRootParameters];
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC RootDesc;
};



class FD3D12RootSignature : public FD3D12AdapterChild
{
public:
	FD3D12RootSignature(FD3D12Adapter* InParent) : FD3D12AdapterChild(InParent)
	{}
	FD3D12RootSignature(FD3D12Adapter* InParent, const class FBoundShaderStateInput& BSS)
		: FD3D12AdapterChild(InParent)
	{
		Init(BSS);
	}

	FD3D12RootSignature(FD3D12Adapter* InParent, const class FD3D12ComputeShader* ComputeShader)
		: FD3D12AdapterChild(InParent)
	{
		Init(ComputeShader);
	}

	void Init(const class FBoundShaderStateInput& BSS);
	void Init(const class FD3D12ComputeShader* ComputeShader);
	void Init(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& InDesc);

	ID3D12RootSignature* GetRootSignature() const { return RootSignature.Get(); }

	uint64 Hash = 0;



	FORCEINLINE uint8 GetCBVDBindSlot(EShaderType Type) const
	{
		switch (Type)
		{
		case ST_Vertex: return BindSlotMap[VS_RootCBVs];
		case ST_Pixel: return BindSlotMap[PS_RootCBVs];
		case ST_Compute: return BindSlotMap[CS_RootCBVs];
		}
	}
	FORCEINLINE uint8 GetCBVDTBindSlot(EShaderType Type) const
	{
		switch (Type)
		{
		case ST_Vertex: return BindSlotMap[VS_CBVs];
		case ST_Pixel: return BindSlotMap[PS_CBVs];
		case ST_Compute: return BindSlotMap[CS_CBVs];
		}
	}
	FORCEINLINE uint8 GetSRVDTBindSlot(EShaderType Type) const
	{
		switch (Type)
		{
		case ST_Vertex: return BindSlotMap[VS_SRVs];
		case ST_Pixel: return BindSlotMap[PS_SRVs];
		}
	}

private:
	void AnalyzeSignature(const D3D12_ROOT_SIGNATURE_DESC1& Desc, uint32 BindingSpace);

	FORCEINLINE void SetCBVDTBindSlot(EShaderType Type, uint8 RootParameterIndex)
	{
		switch (Type)
		{
		case ST_Vertex:
			BindSlotMap[VS_CBVs] = RootParameterIndex;
			break;
		case ST_Pixel:
			BindSlotMap[PS_CBVs] = RootParameterIndex;
			break;
		case ST_Compute:
			BindSlotMap[CS_CBVs] = RootParameterIndex;
			break;
		}
	}
	FORCEINLINE void SetSRVDTBindSlot(EShaderType Type, uint8 RootParameterIndex)
	{
		switch (Type)
		{
		case ST_Vertex:
			BindSlotMap[VS_SRVs] = RootParameterIndex;
			break;
		case ST_Pixel:
			BindSlotMap[PS_SRVs] = RootParameterIndex;
			break;
		}
	}
	FORCEINLINE void SetCBVDBindSlot(EShaderType Type, uint8 RootParameterIndex)
	{
		switch (Type)
		{
		case ST_Vertex:
			BindSlotMap[VS_RootCBVs] = RootParameterIndex;
			break;
		case ST_Pixel:
			BindSlotMap[PS_RootCBVs] = RootParameterIndex;
			break;
		case ST_Compute:
			BindSlotMap[CS_RootCBVs] = RootParameterIndex;
		}
	}

private:
	TRefCountPtr<ID3DBlob> RootSignatureBlob;
	TRefCountPtr<ID3D12RootSignature> RootSignature;

	uint8 BindSlotMap[RPK_Count];
};


class FD3D12RootSignatureManager : public FD3D12AdapterChild
{
public:
	FD3D12RootSignatureManager(FD3D12Adapter* InParent): FD3D12AdapterChild(InParent) {}

	FD3D12RootSignature* GetRootSignature(const class FBoundShaderStateInput& BSS);
	FD3D12RootSignature* GetRootSignature(const class FD3D12ComputeShader* ComputeShader);


private:
	FD3D12RootSignature* CreateRootSignature(const class FBoundShaderStateInput& BSS);
	FD3D12RootSignature* CreateRootSignature(const class FD3D12ComputeShader* ComputeShader);

	TUnordoredMap<FBoundShaderStateInput, FD3D12RootSignature*> RootSignatureMap;
	TMap<uint64, FD3D12RootSignature*> ComputeRootSignatureMap;

};