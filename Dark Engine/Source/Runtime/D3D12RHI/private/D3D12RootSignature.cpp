#include "D3D12RootSignature.h"
#include "RHIResources.h"
#include "D3D12Adapter.h"
#include "d3dx12.h"
#include "D3D12Util.h"


FD3D12RootSignature* FD3D12RootSignatureManager::GetRootSignature(FGraphicsPipelineStateInitializer* Initalizer)
{
	auto It = RootSignatureMap.find(Initalizer);

	if (It == RootSignatureMap.end())
	{
		return CreateRootSignature(Initalizer);
	}

	return It->second;
}

FD3D12RootSignature* FD3D12RootSignatureManager::CreateRootSignature(FGraphicsPipelineStateInitializer* Initalizer)
{
	FD3D12RootSignature* NewRootSignature = new FD3D12RootSignature(GetParentAdapter(), Initalizer);

	RootSignatureMap.emplace(Initalizer, NewRootSignature);
	return NewRootSignature;
}

void FD3D12RootSignature::Init(FGraphicsPipelineStateInitializer* Initalizer)
{



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
