#include "D3D12RHI.h"
#include "../D3D12RHIPrivate.h"
#include "../D3D12Adapter.h"
#include "../D3D12RHICommon.h"
#include <Templates/DarkTemplate.h>
#include "Misc/Paths.h"
#include "wrl.h"
#include "Misc/EngineVersion.h"
#include "future"

#include <filesystem>
#include <shlobj.h>
#include <Logger.h>

constexpr const bool bNvidiaStreamline = D3D12_NVIDIA_STREAMLINE;

HMODULE NVStreamLine = NULL;

extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = D3D12_AGILITY_VERSION; }

extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }

DECLARE_LOG_CATEGORY(D3D12Streamline, Log);

//static void StreamlineCallbackLog(sl::LogType LogType, const char* Msg)
//{
//	switch (LogType)
//	{
//	case sl::LogType::eInfo:
//		DE_LOG(D3D12Streamline, Log, TEXT("%s"), *FString(Msg));
//		break;
//	case sl::LogType::eWarn:
//		DE_LOG(D3D12Streamline, Warning, TEXT("%s"), *FString(Msg));
//		break;
//	case sl::LogType::eError:
//		DE_LOG(D3D12Streamline, Error, TEXT("%s"), *FString(Msg));
//		break;
//	}
//
//}

static std::wstring GetLatestWinPixGpuCapturerPath_Cpp17()
{
	LPWSTR programFilesPath = nullptr;
	SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &programFilesPath);

	std::filesystem::path pixInstallationPath = programFilesPath;
	pixInstallationPath /= "Microsoft PIX";

	std::wstring newestVersionFound;

	for (auto const& directory_entry : std::filesystem::directory_iterator(pixInstallationPath))
	{
		if (directory_entry.is_directory())
		{
			if (newestVersionFound.empty() || newestVersionFound < directory_entry.path().filename().c_str())
			{
				newestVersionFound = directory_entry.path().filename().c_str();
			}
		}
	}

	if (newestVersionFound.empty())
	{
		// TODO: Error, no PIX installation found
	}

	return pixInstallationPath / newestVersionFound / L"WinPixGpuCapturer.dll";
}

FD3D12DynamicRHI* GD3D12RHI = nullptr;




FD3D12DynamicRHIModule::FD3D12DynamicRHIModule()
{
#if USE_PIX
	if (GetModuleHandle(TEXT("WinPixGpuCapturer.dll")) == 0)
	{
		if (LoadLibrary(GetLatestWinPixGpuCapturerPath_Cpp17().c_str()))
		{
		//	Logger::log("[D3D12RHIModule] WinPixGpuCapturer.dll was loaded");
		}
	}
#endif
	/*if (bNvidiaStreamline)
	{
		sl::Preferences Pref = {};
		sl::Feature Features[] = { NULL };
		if (D3D12_DEBUG)
		{
			Pref.logLevel = sl::LogLevel::eDefault;
			Pref.logMessageCallback = StreamlineCallbackLog;
		}

		Pref.flags |= sl::PreferenceFlags::eAllowOTA | sl::PreferenceFlags::eLoadDownloadedPlugins;
		Pref.engine = sl::EngineType::eCustom;
		Pref.engineVersion = GetStringVersionA();
		Pref.renderAPI = sl::RenderAPI::eD3D12;
		Pref.featuresToLoad = Features;
		Pref.numFeaturesToLoad = DE_ARRAY_COUNT(Features);
		

		sl::Result SlResult;
		if (SL_SUCCEEDED(SlResult, slInit(Pref)))
		{
			DE_LOG(D3D12Streamline, Log, TEXT("Streamline init success"));
		}

	}*/


	FindAdapter();
}

template <typename T>
bool InternalUpgradeInterface(T** Interface)
{
	/*if (bNvidiaStreamline)
	{
		void* NewInterface;
		sl::Result Result;
		if (SL_SUCCEEDED(Result, slGetNativeInterface(*Interface, &NewInterface)))
		{
			*Interface = static_cast<T*>(NewInterface);
			return true;
		}
	}*/
	return false;
}


bool UpgradeInterface(void** Interface)
{
	//if (bNvidiaStreamline)
	//{
	//	void* NewInterface;
	//	sl::Result Result;// = slGetNativeInterface(*Interface, &NewInterface);
	//	//Result = slUpgradeInterface(Interface);
	//	if (SL_SUCCEEDED(Result, slUpgradeInterface(Interface)))
	//	{
	//		return true;
	//	}
	//	if (SL_SUCCEEDED(Result, Result = slGetNativeInterface(*Interface, &NewInterface)))
	//	{
	//		*Interface = NewInterface;
	//		return true;
	//	}
	//}
	return false;
}


FDynamicRHI* FD3D12DynamicRHIModule::CreateRHI()
{
	if(ChosenAdapters.GetSize() != 0)
		GD3D12RHI = new FD3D12DynamicRHI(ChosenAdapters, bNvidiaStreamline);

	return GD3D12RHI;
}

void FD3D12DynamicRHIModule::Shutdown()
{
	for (auto& i : ChosenAdapters)
	{
		//i.reset();
		i->Destroy();
	}
	if (GD3D12RHI)
	{
		delete GD3D12RHI;
		GD3D12RHI = nullptr;
	}

	if (bNvidiaStreamline)
		FreeLibrary(NVStreamLine);
}


void GetResourceTiers(ID3D12Device* Device, D3D12_RESOURCE_BINDING_TIER& OutResourceBindingTier,
	D3D12_RESOURCE_HEAP_TIER& OutResourceHeapTier)
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS D3D12Caps = {};
	Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &D3D12Caps, sizeof(D3D12Caps));

	OutResourceBindingTier = D3D12Caps.ResourceBindingTier;
	OutResourceHeapTier = D3D12Caps.ResourceHeapTier;
}



D3D_SHADER_MODEL FindHighestShaderModel(ID3D12Device* Device)
{
	const D3D_SHADER_MODEL ShadersModelsForCheck[] =
	{	
		D3D_SHADER_MODEL_6_9,
		D3D_SHADER_MODEL_6_8,
		D3D_SHADER_MODEL_6_7,
		D3D_SHADER_MODEL_6_6,
		D3D_SHADER_MODEL_6_5,
		D3D_SHADER_MODEL_6_4,
		D3D_SHADER_MODEL_6_3,
		D3D_SHADER_MODEL_6_2,
		D3D_SHADER_MODEL_6_1,
		D3D_SHADER_MODEL_6_0,
		D3D_SHADER_MODEL_5_1
	};

	D3D12_FEATURE_DATA_SHADER_MODEL FeatureShaderModel = {};
	for (const auto ShaderModelsForCheck : ShadersModelsForCheck)
	{
		FeatureShaderModel.HighestShaderModel = ShaderModelsForCheck;
		
		if (SUCCEEDED(Device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &FeatureShaderModel, sizeof(FeatureShaderModel))))
		{
			return FeatureShaderModel.HighestShaderModel;
		}
	}

	return D3D_SHADER_MODEL_5_1;

}



D3D_FEATURE_LEVEL FindHighestFeatureLevel(ID3D12Device* Device, D3D_FEATURE_LEVEL MinFeatureLevel)
{
	const D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};

	D3D12_FEATURE_DATA_FEATURE_LEVELS FeatureLevel;
	FeatureLevel.pFeatureLevelsRequested = FeatureLevels;
	FeatureLevel.NumFeatureLevels = DE_ARRAY_COUNT(FeatureLevels);

	if (SUCCEEDED(Device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &FeatureLevel, sizeof(FeatureLevel))))
	{
		return FeatureLevel.MaxSupportedFeatureLevel;
	}
	return MinFeatureLevel;
}






bool TestD3D12CreateDevice(IDXGIAdapter* Adapter, FD3D12DeviceBasicInfo& BasicInfo)
{
	ID3D12Device* Device = nullptr;
	D3D_FEATURE_LEVEL MinFeatureLevel = D3D_FEATURE_LEVEL_12_0;
	const HRESULT D3D12CreateDeviceResult = D3D12CreateDevice(Adapter, MinFeatureLevel, IID_PPV_ARGS(&Device));
	if (SUCCEEDED(D3D12CreateDeviceResult))
	{
		BasicInfo.MaxFeatureLevel = FindHighestFeatureLevel(Device, MinFeatureLevel);
		BasicInfo.MaxShaderModel = FindHighestShaderModel(Device);
		BasicInfo.NumDeviceNodes = Device->GetNodeCount();
		GetResourceTiers(Device, BasicInfo.ResourceBindingTier,
			BasicInfo.ResourceHeapTier);

		Device->Release();
		return true;
	}

	return false;
}




void FD3D12DynamicRHIModule::FindAdapter()
{
	if (ChosenAdapters.GetSize() != 0)
	{
		return;
	}



	//HRESULT hr = D3D12EnableExperimentalFeatures(1, &D3D12ExperimentalShaderModels, nullptr, nullptr);
	Microsoft::WRL::ComPtr<IDXGIFactory7> DXGIFactory;
	CreateDXGIFactory2(0, IID_PPV_ARGS(&DXGIFactory));

	//UpgradeInterface(&DXGIFactory);

	if (!DXGIFactory)
	{
		return;
	}
	Microsoft::WRL::ComPtr<IDXGIAdapter> TempAdapter;

	FD3D12AdapterDesc BestMemoryAdapter;

	for (uint32 AdapterIndex = 0; FD3D12AdapterDesc::EnumAdapters(AdapterIndex, DXGI_GPU_PREFERENCE_UNSPECIFIED,
		DXGIFactory.Get(), &TempAdapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
	{
		if (TempAdapter)
		{
			FD3D12DeviceBasicInfo DeviceInfo;
			auto TestDevice = std::async([&TempAdapter, &DeviceInfo]() -> bool {return TestD3D12CreateDevice(TempAdapter.Get(), DeviceInfo); });
			auto Status = TestDevice.wait_for(std::chrono::seconds(1));
			if (Status == std::future_status::ready && TestDevice.get())
			{
				if (DeviceInfo.NumDeviceNodes <= 0)
				{
					return;
				}

				DXGI_ADAPTER_DESC AdapterDesc = {};
				TempAdapter->GetDesc(&AdapterDesc);

				FD3D12AdapterDesc CurrentAdapter(AdapterDesc, AdapterIndex, DeviceInfo);

				if (CurrentAdapter.Desc.DedicatedVideoMemory > BestMemoryAdapter.Desc.DedicatedVideoMemory)
				{
					BestMemoryAdapter = CurrentAdapter;
				}
			}

		}
	}


	TSharedPtr<FD3D12Adapter> NewAdapter;

	if (BestMemoryAdapter.IsValid())
	{
		NewAdapter = std::make_shared<FD3D12Adapter>(BestMemoryAdapter);
		ChosenAdapters.Add(NewAdapter);
		DE_LOG(D3D12RHI, Log, TEXT("Choose device: %s with %iMB"), BestMemoryAdapter.Desc.Description,
			BestMemoryAdapter.Desc.DedicatedVideoMemory / (1024 * 1024));
		DE_LOG(D3D12RHI, Log, TEXT("Max shader model: %s"), *ToStringD3DShaderModel(BestMemoryAdapter.BasicInfo.MaxShaderModel));
	}
	else
	{
		DE_LOG(D3D12RHI, Fatal, TEXT("This system not have supported GPU. %s(%i)"), BestMemoryAdapter.Desc.Description,
			BestMemoryAdapter.Desc.DedicatedVideoMemory / (1024 * 1024));
	}


	if (ChosenAdapters.GetSize() > 0 && ChosenAdapters[0]->GetDesc().IsValid())
	{
		const DXGI_ADAPTER_DESC& AdapterDesc = ChosenAdapters[0]->GetAdapterDesc();

		//...
	}

}