#pragma once
#include "Templates/DarkTemplate.h"
#include "D3D12RHIPrivate.h"
#include "Containers/StaticArray.h"
#include "D3D12Viewport.h"


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
	FD3D12AdapterDesc(const DXGI_ADAPTER_DESC& InDesc, int32 InAdapterIndex, const FD3D12DeviceBasicInfo& BasicInfo);

	bool IsValid() const;


	static HRESULT EnumAdapters(int32 AdapterIndex, DXGI_GPU_PREFERENCE GPUPreference,
								IDXGIFactory7* DXGIFactory, IDXGIAdapter** TempAdapter);




	DXGI_ADAPTER_DESC Desc = {};
	FD3D12DeviceBasicInfo BasicInfo;
	int32 AdapterIndex = -1;
	D3D_FEATURE_LEVEL MaxSupportFeatureLevel;

};






class FD3D12Adapter : public FNoncopyble
{
	friend class FD3D12DynamicRHI;
public:
	FD3D12Adapter(FD3D12AdapterDesc& DescIn);
	~FD3D12Adapter();

	void Destroy();

	void InitializeDevices();

	bool CheckFeaturesOrCrash();


	D3D12MA::Allocator* GetMemoryAllocator() const { return MemoryAllocator.Get(); }

	const int32 GetAdapterIndex() const { return Desc.AdapterIndex; }
	const D3D_FEATURE_LEVEL GetFeatureLevel() const { return Desc.MaxSupportFeatureLevel; }
	ID3D12Device* GetD3DDevice() const { return RootDevice.Get(); }
	ID3D12Device2* GetD3DDevice2() const { return RootDevice2.Get(); }
	ID3D12Device10* GetD3DDevice10() const { return RootDevice10.Get(); }

	const FD3D12AdapterDesc& GetDesc() const { return Desc; }
	const DXGI_ADAPTER_DESC& GetAdapterDesc() const { return Desc.Desc; }

	IDXGIAdapter4* GetAdapter() { return DXGIAdapter.Get(); }

	IDXGIFactory7* GetDXGIFactory()	const { return DXGIFactory.Get(); }

	void CreateRootDevice(bool bWithDebug = false);
	void SetDebugMessages(bool bOn);
	void CreateDXGIFactory(bool bWithDebug = false);

#if D3D12_USING_DIRECTSR
	const TArray<DSR_SUPERRES_VARIANT_DESC>& GetDSRDescs() const { return AvailableSuperResolutions; }
#endif

	FD3D12Device* GetDevice(uint32 Index = 0) const { return Devices[Index].get(); }

	TArray<FD3D12Viewport*>& GetViewports() { return Viewports; }

	void SetDrawingViewport(FD3D12Viewport* InViewport) { DrawingViewport = InViewport; }
	FD3D12Viewport* GetDrawingViewport() const { return DrawingViewport; }

	class FD3D12RootSignatureManager* const RootSignatureManager;

	CD3DX12FeatureSupport FeatureSupport;

private:
	void GetAvailableDynamicSuperResolutions();

private:
	FD3D12AdapterDesc Desc = {};
	ComPtr<ID3D12Device> RootDevice;
	ComPtr<ID3D12Device2> RootDevice2;
	ComPtr<ID3D12Device10> RootDevice10;
#if D3D12_USING_MEMORY_ALLOCATOR
	ComPtr<D3D12MA::Allocator> MemoryAllocator;
#endif
	ComPtr<IDXGIFactory7> DXGIFactory;
	ComPtr<IDXGIAdapter4> DXGIAdapter;
	ComPtr<ID3D12DSRDeviceFactory> DSRDeviceFactory;
	TStaticArray<TSharedPtr<FD3D12Device>, 1> Devices;
#if D3D12_USING_DIRECTSR
	TArray<DSR_SUPERRES_VARIANT_DESC> AvailableSuperResolutions;
	ComPtr<IDSRDevice> DSRDevice;
#endif

	ComPtr<ID3D12InfoQueue1> InfoQueue;

	bool bDebugDevice = false;
	bool bDeviceCreated;
	bool bDestroyed = false;
	bool bDSRDescAvailable = false;

	DWORD CallbackCoockie;
	bool bCallbackEnable = false;

	TArray<FD3D12Viewport*>	Viewports;

	FD3D12Viewport* DrawingViewport;



};