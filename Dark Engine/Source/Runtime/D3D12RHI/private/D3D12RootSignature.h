#pragma once

#include "D3D12RHICommon.h"
#include "Containers/UnordoredMap.h"
#include "Containers/Array.h"



class FD3D12RootSignatureDesc
{
public:
	explicit FD3D12RootSignatureDesc(const FBoundShaderStateInput& BSS);

	static constexpr uint32 MaxRootParameters = 32;

	FORCEINLINE const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& GetDesc() const { return RootDesc; }

private:
	uint32 RootParameterSize;
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

	void Init(const class FBoundShaderStateInput& BSS);
	void Init(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& InDesc);

	ID3D12RootSignature* GetRootSignature() const { return RootSignature.Get(); }


private:
	TRefCountPtr<ID3DBlob> RootSignatureBlob;
	TRefCountPtr<ID3D12RootSignature> RootSignature;



};


class FD3D12RootSignatureManager : public FD3D12AdapterChild
{
public:
	FD3D12RootSignatureManager(FD3D12Adapter* InParent): FD3D12AdapterChild(InParent) {}

	FD3D12RootSignature* GetRootSignature(const class FBoundShaderStateInput& BSS);


private:
	FD3D12RootSignature* CreateRootSignature(const class FBoundShaderStateInput& BSS);

	TUnordoredMap<FBoundShaderStateInput, FD3D12RootSignature*> RootSignatureMap;

};