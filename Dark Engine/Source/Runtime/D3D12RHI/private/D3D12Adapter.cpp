#include "D3D12Adapter.h"
#include "D3D12Device.h"
#include "Logging/LogMacros.h"
#include <D3D12Util.h>


FD3D12AdapterDesc::FD3D12AdapterDesc() = default;
FD3D12AdapterDesc::FD3D12AdapterDesc(const DXGI_ADAPTER_DESC& InDesc, int32 InAdapterIndex)
	: Desc(InDesc)
	, AdapterIndex(InAdapterIndex)
	, MaxSupportFeatureLevel(D3D_FEATURE_LEVEL_12_0 /*temp*/)
	//, RootSignatureManager(this), PipelineStateCache(this), DefaultContextRedirector(this)
{




}

bool FD3D12AdapterDesc::IsValid() const
{
	return MaxSupportFeatureLevel != (D3D_FEATURE_LEVEL)(0) && AdapterIndex >= 0;
}


FD3D12Adapter::FD3D12Adapter(FD3D12AdapterDesc& DescIn)
	: Desc(DescIn)
	, bDeviceCreated(false)
{

}

FD3D12Adapter::~FD3D12Adapter()
{



}



HRESULT FD3D12AdapterDesc::EnumAdapters(int32 InAdapterIndex, DXGI_GPU_PREFERENCE GPUPreference,
	IDXGIFactory7* DXGIFactory, IDXGIAdapter** TempAdapter)
{
	if (!DXGIFactory || GPUPreference == DXGI_GPU_PREFERENCE_UNSPECIFIED)
	{
		return DXGIFactory->EnumAdapters(InAdapterIndex, TempAdapter);
	}
	return DXGIFactory->EnumAdapterByGpuPreference(InAdapterIndex, GPUPreference,
		IID_PPV_ARGS(TempAdapter));
}

void FD3D12Adapter::InitializeDevices()
{
	if (!RootDevice)
	{
		CreateRootDevice(D3D12_DEBUG);
	}

	Devices[0] = new FD3D12Device(this);



}


void FD3D12Adapter::CreateRootDevice(bool bWithDebug)
{
	if (bWithDebug)
	{
		ComPtr<ID3D12Debug> DebugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
		{
			DebugController->EnableDebugLayer();
			bDebugDevice = true;

			ComPtr<ID3D12Debug5> DebugController5;
			DebugController->QueryInterface(IID_PPV_ARGS(&DebugController5));

			DebugController5->SetEnableGPUBasedValidation(TRUE);
			DebugController5->SetEnableAutoName(TRUE);

			DE_LOG(D3D12RHI, Log, TEXT("Debug layer enable"));
		}
		else
		{
			DE_LOG(D3D12RHI, Warning, TEXT("Debug layer can not on"));
		}
	}

	CreateDXGIFactory2(0, IID_PPV_ARGS(&DXGIFactory));

	ComPtr<IDXGIAdapter> TempAdapter;


	DXGIFactory->EnumAdapters(Desc.AdapterIndex, &TempAdapter);


	if (!bDeviceCreated)
	{
		DXCall(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&RootDevice)));
	}
	else
	{
		DE_LOG(D3D12RHI, Error, TEXT("Device not Created"));
	}

}

