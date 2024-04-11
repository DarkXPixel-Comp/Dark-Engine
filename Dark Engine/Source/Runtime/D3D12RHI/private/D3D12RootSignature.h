#pragma once

#include "D3D12RHICommon.h"
#include "Containers/UnordoredMap.h"

class FD3D12RootSignature : public FD3D12AdapterChild
{
public:
	FD3D12RootSignature(FD3D12Adapter* InParent) : FD3D12AdapterChild(InParent)
	{}
	FD3D12RootSignature(FD3D12Adapter* InParent, class FGraphicsPipelineStateInitializer* Initalizer)
		: FD3D12AdapterChild(InParent)
	{
		//Init();
	}

	void Init(class FGraphicsPipelineStateInitializer* Initalizer);
	void Init(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& InDesc);


private:
	TRefCountPtr<ID3DBlob> RootSignatureBlob;
	TRefCountPtr<ID3D12RootSignature> RootSignature;



};


class FD3D12RootSignatureManager : public FD3D12AdapterChild
{
public:
	FD3D12RootSignatureManager(FD3D12Adapter* InParent): FD3D12AdapterChild(InParent) {}

	FD3D12RootSignature* GetRootSignature(class FGraphicsPipelineStateInitializer* Initalizer);


private:
	FD3D12RootSignature* CreateRootSignature(class FGraphicsPipelineStateInitializer* Initalizer);

	TUnordoredMap<class FGraphicsPipelineStateInitializer*, FD3D12RootSignature*> RootSignatureMap;

};