#pragma once
#include "Templates/DarkTemplate.h"
#include "D3D12RHIPrivate.h"


#include <wrl.h>

using Microsoft::WRL::ComPtr;


struct FD3D12DeviceBasicInfo
{
	D3D_FEATURE_LEVEL			MaxFeatureLevel;
	D3D_SHADER_MODEL			MaxShaderModel;
	D3D12_RESOURCE_BINDING_TIER	ResourceBindingTier;
	D3D12_RESOURCE_HEAP_TIER	ResourceHeapTier;
	uint32						NumDeviceNodes;
};



struct FD3D12AdapterDesc
{
	FD3D12AdapterDesc();
	FD3D12AdapterDesc(const DXGI_ADAPTER_DESC& InDesc, int32 InAdapterIndex);

	bool IsValid() const;


	static HRESULT EnumAdapters(int32 AdapterIndex, DXGI_GPU_PREFERENCE GPUPreference,
								IDXGIFactory7* DXGIFactory, IDXGIAdapter** TempAdapter);




	DXGI_ADAPTER_DESC Desc = {};
	int32 AdapterIndex = -1;
	D3D_FEATURE_LEVEL MaxSupportFeatureLevel;

};






class FD3D12Adapter : public FNoncopyble
{
	friend class FD3D12DynamicRHI;
public:
	FD3D12Adapter(FD3D12AdapterDesc& DescIn);
	~FD3D12Adapter();

	void InitializeDevices();


	const int32 GetAdapterIndex() const { return Desc.AdapterIndex; }
	const D3D_FEATURE_LEVEL GetFeatureLevel() const { return Desc.MaxSupportFeatureLevel; }
	ID3D12Device10* GetD3DDevice() const { return RootDevice.Get(); }
	const FD3D12AdapterDesc& GetDesc() const { return Desc; }
	const DXGI_ADAPTER_DESC& GetAdapterDesc() const { return Desc.Desc; }

	IDXGIAdapter4* GetAdapter() { return DXGIAdapter.Get(); }

	IDXGIFactory7* GetDXGIFactory()	const { return DXGIFactory.Get(); }

	void CreateRootDevice(bool bWithDebug = false);
	void CreateDXGIFactory(bool bWithDebug = false);


private:
	FD3D12AdapterDesc Desc = {};
	ComPtr<ID3D12Device10> RootDevice;
	ComPtr<IDXGIFactory7> DXGIFactory;
	ComPtr<IDXGIAdapter4> DXGIAdapter;

	bool bDebugDevice;
	bool bDeviceCreated;



};