#include "D3D12Adapter.h"
#include "D3D12Device.h"
#include "Logging/LogMacros.h"
#include "CoreGlobals.h"
#include <D3D12Util.h>
#include "D3D12RootSignature.h"
#include "D3D12PipelineState.h"
#include "sl.h"


DECLARE_LOG_CATEGORY(D3D12Callback, Display);



/*DEFINE_ENUM_FLAG_OPERATORS(D3D12_MESSAGE_CALLBACK_FLAGS)
typedef void ( __stdcall *D3D12MessageFunc )( 
    D3D12_MESSAGE_CATEGORY Category,
    D3D12_MESSAGE_SEVERITY Severity,
    D3D12_MESSAGE_ID ID,
    LPCSTR pDescription,
    void *pContext);
*/

void D3D12MessageCallBack(D3D12_MESSAGE_CATEGORY Category, D3D12_MESSAGE_SEVERITY Severity,
	D3D12_MESSAGE_ID ID, LPCSTR Description, void* pContext)
{
	switch (Severity)
	{
	case D3D12_MESSAGE_SEVERITY_CORRUPTION:
		DE_LOG(D3D12Callback, Display, TEXT("%s"), *FString(Description));
		break;
	case D3D12_MESSAGE_SEVERITY_ERROR:
		DE_LOG(D3D12Callback, Error, TEXT("%s"), *FString(Description));
		break;
	case D3D12_MESSAGE_SEVERITY_WARNING:
		DE_LOG(D3D12Callback, Warning, TEXT("%s"), *FString(Description));
		break;
	case D3D12_MESSAGE_SEVERITY_INFO:
		DE_LOG(D3D12Callback, Log, TEXT("%s"), *FString(Description));
		break;
	case D3D12_MESSAGE_SEVERITY_MESSAGE:
		DE_LOG(D3D12Callback, Log, TEXT("%s"), *FString(Description));
		break;
	}
}





FD3D12AdapterDesc::FD3D12AdapterDesc() = default;
FD3D12AdapterDesc::FD3D12AdapterDesc(const DXGI_ADAPTER_DESC& InDesc, int32 InAdapterIndex, const FD3D12DeviceBasicInfo& InBasicInfo)
	: Desc(InDesc)
	, AdapterIndex(InAdapterIndex)
	, MaxSupportFeatureLevel(D3D_FEATURE_LEVEL_12_0 /*temp*/),
	BasicInfo(InBasicInfo)
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
	, RootSignatureManager(new FD3D12RootSignatureManager(this))
{

}

FD3D12Adapter::~FD3D12Adapter()
{
	if (!bDestroyed)
		Destroy();
}

void FD3D12Adapter::Destroy()
{
	for (auto& Device : Devices)
	{
		Device->Destroy();
	}
	delete RootSignatureManager;

	bDestroyed = true;
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

	Devices[0] = MakeShareble(new FD3D12Device(this));

	FeatureSupport.Init(RootDevice.Get());

	CheckFeaturesOrCrash();
}

bool FD3D12Adapter::CheckFeaturesOrCrash()
{
	bool Result = true;

	Result = Result && FeatureSupport.EnhancedBarriersSupported();

	if (!Result)
	{
		FPlatformMisc::CreateMessageBoxError(TEXT("Error check features. Your device not support, try update driver"), TEXT("Error D3D12"));
		Destroy();
		RequestExit();
		return false;
	}
	return true;
}

#undef GetMessage

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
			DebugController5->SetEnableSynchronizedCommandQueueValidation(TRUE);
			DebugController5->SetEnableAutoName(TRUE);
						
			DE_LOG(D3D12RHI, Log, TEXT("Debug layer enable"));
		}
		else
		{
			DE_LOG(D3D12RHI, Warning, TEXT("Debug layer can not on"));
		}
	}

	CreateDXGIFactory2(0, IID_PPV_ARGS(&DXGIFactory));
	UpgradeInterface(&DXGIFactory);

	ComPtr<IDXGIAdapter> TempAdapter;


	DXGIFactory->EnumAdapters(Desc.AdapterIndex, &TempAdapter);


	if (!bDeviceCreated)
	{
		DXCall(D3D12CreateDevice(TempAdapter.Get(), D3D_FEATURE_LEVEL_12_0,
			IID_PPV_ARGS(&RootDevice)));
		if (bNvidiaStreamline)
		{
			slSetD3DDevice(RootDevice.Get());
		}

		RootDevice->QueryInterface(IID_PPV_ARGS(&RootDevice2));
		RootDevice->QueryInterface(IID_PPV_ARGS(&RootDevice10));
	}
	else
	{
		DE_LOG(D3D12RHI, Error, TEXT("Device not Created"));
	}


	if (bWithDebug)
	{
		RootDevice->QueryInterface(IID_PPV_ARGS(&InfoQueue));
		if (!InfoQueue)
		{
			DE_LOG(D3D12RHI, Warning, TEXT("This device not support d3d12 callbacks"));
		}
	}

	IConsoleVariable* Var = GGlobalConsole.FindConsoleVariable(TEXT("r.D3D12.DebugMessages"));
	if (Var && Var->IsVariableBool())
	{
		Var->SetOnChangedCallback([this](IConsoleVariable* Obj) 
			{
				SetDebugMessages(Obj->GetBool());
			});
		SetDebugMessages(Var->GetBool());
	}
}

void FD3D12Adapter::SetDebugMessages(bool bOn)
{
	if (InfoQueue && bOn != bCallbackEnable)
	{
		if (bOn)
		{
			InfoQueue->RegisterMessageCallback(D3D12MessageCallBack, D3D12_MESSAGE_CALLBACK_FLAG_NONE,
				nullptr, &CallbackCoockie);
			bCallbackEnable = true;
		}
		else
		{
			InfoQueue->UnregisterMessageCallback(CallbackCoockie);
			bCallbackEnable = false;
		}
	}
}


