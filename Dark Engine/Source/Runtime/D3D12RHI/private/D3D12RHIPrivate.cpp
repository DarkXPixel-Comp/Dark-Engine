#include "D3D12RHIPrivate.h"
#include <Misc/AssertionMacros.h>
#include "RHICommandList.h"
#include "D3D12CommandContext.h"
#include "imgui_impl_dx12.h"
#include <D3D12Util.h>
#include "D3D12View.h"

#include "DDSTextureLoader.h"
#include "ResourceUploadBatch.h"
#include "Misc/Paths.h"
#include "D3D12Shader.h"
#include "D3D12RootSignature.h"
#include "D3D12PipelineState.h"

FD3D12DynamicRHI* FD3D12DynamicRHI::SingleD3D12RHI = nullptr;

FD3D12DynamicRHI::FD3D12DynamicRHI(const TArray<TSharedPtr<FD3D12Adapter>>& InChosenAdapters)
	: ChosenAdapters(InChosenAdapters)
{
	if (SingleD3D12RHI)
	{
		return;
	}

	SingleD3D12RHI = this;

	FeatureLevel = GetAdapter().GetFeatureLevel();




}

FD3D12DynamicRHI::~FD3D12DynamicRHI()
{
}

void FD3D12DynamicRHI::Init()
{
	DE_LOG(D3D12RHI, Log, TEXT("Start init"));

	for (auto& Adapter : ChosenAdapters)
	{
		check(Adapter->GetDesc().IsValid());
		Adapter->InitializeDevices();
	}

	FD3D12Adapter& Adapter = GetAdapter();

	ImGuiDescriptorHeap = CreateDescriptorHeap(
		Adapter.GetDevice(),
		TEXT("ImGuiDescriptorHeap"),
		ERHIDescriptorHeapType::Standart,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	);
	DE_LOG(D3D12RHI, Log, TEXT("Create IMGUI Heap"));


	GRHICommandList.GetImmediateCommandList().InitializeContexts();

	DE_LOG(D3D12RHI, Log, TEXT("Initialize Contexts"));

	DE_LOG(D3D12RHI, Log, TEXT("Finish init"));
}

void FD3D12DynamicRHI::Shutdown()
{
	for (auto& i : VertexDeclarationMap)
	{
		i.second->Release();
	}
}

TSharedPtr<FRHIViewport> FD3D12DynamicRHI::RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen)
{
	FD3D12Viewport* RenderingViewport = new FD3D12Viewport(&GetAdapter(), HWND(WindowHandle), SizeX, SizeY,
		bIsFullscreen, EPixelFormat::PF_R8G8B8A8_UNORM);
	RenderingViewport->Init();

#ifdef IMGUI
	FD3D12BindlessDescriptorManager& DescriptorManager = GetAdapter().GetDevice()->GetBindlessDescriptorManager();
	ImGuiDescriptorHandle = DescriptorManager.Allocate(ERHIDescriptorHeapType::Standart);

	ImGui_ImplDX12_Init(GetAdapter().GetD3DDevice(), RenderingViewport->GetCountBackBuffers(),
		GetDXGIFormat(EPixelFormat::PF_R8G8B8A8_UNORM),
		DescriptorManager.GetHeap(ERHIDescriptorHeapType::Standart)->GetHeap(),
		DescriptorManager.GetHeap(ERHIDescriptorHeapType::Standart)->GetCPUSlotHandle(ImGuiDescriptorHandle.GetIndex()),
		DescriptorManager.GetGpuHandle(ImGuiDescriptorHandle));
#endif

	return MakeShareble(RenderingViewport);
}

FD3D12CommandContext* FD3D12DynamicRHI::CreateCommandContext(FD3D12Device* InParent, ED3D12QueueType InQueueType, bool InIsDefaultContext)
{
	return new FD3D12CommandContext(InParent, InQueueType, InIsDefaultContext);
}

FRHITexture* FD3D12DynamicRHI::RHIGetViewportBackBuffer(FRHIViewport* Viewport)
{
	FD3D12Viewport* D3DViewport = static_cast<FD3D12Viewport*>(Viewport);
	FRHITexture* Result = static_cast<FRHITexture*>(D3DViewport->GetCurrentBackBuffer());
	return Result;
}


TRefCountPtr<FRHIVertexDeclaration> FD3D12DynamicRHI::RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements)
{
	FD3D12VertexElements D3DElements(Elements.Num());

	uint64 Hash = 0;
	for (const auto& i : Elements)
	{
		hash_combine(Hash, i);
	}

	auto It = VertexDeclarationMap.find(Hash);

	if (It != VertexDeclarationMap.end())
	{
		return It->second;
	}

	for (uint32 i = 0; i < Elements.Num(); ++i)
	{
		D3D12_INPUT_ELEMENT_DESC& Element = D3DElements[i];
		
		switch (Elements[i].Type)
		{
		case VET_Float1:
			Element.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case VET_Float2:
			Element.Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case VET_Float3:
			Element.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case VET_Float4:
			Element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case VET_Color:
			Element.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case VET_Short2:
			Element.Format = DXGI_FORMAT_R16G16_SINT;
			break;
		case VET_Short3:
			Element.Format = DXGI_FORMAT_R32G32_SINT;
			break;
		case VET_Short4:
			Element.Format = DXGI_FORMAT_R16G16B16A16_SINT;
			break;
		case VET_UInt:
			Element.Format = DXGI_FORMAT_R32_UINT;
			break;
		default:
			break;
		}

		Element.SemanticIndex = Elements[i].AttributeIndex;
		Element.InputSlotClass = Elements[i].bUseInstanceIndex ? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA : D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		Element.InstanceDataStepRate = Elements[i].bUseInstanceIndex ? 1 : 0;
		Element.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		Element.SemanticName = "Attribute";
		Element.InputSlot = 0;
	}
	FD3D12VertexDeclaration* VertexDeclaration = new FD3D12VertexDeclaration(D3DElements);

	VertexDeclarationMap.emplace(Hash, VertexDeclaration);
	VertexDeclaration->AddRef();

	return VertexDeclaration;
}

TRefCountPtr<FRHIVertexShader> FD3D12DynamicRHI::RHICreateVertexShader(const TArray<uint8>& Code, const FShaderBounds& Bounds)
{
	FD3D12VertexShader* Result = CreateStandartShader<FD3D12VertexShader>(Code);
	Result->ResourceCounts = Bounds;
	return Result;
}

TRefCountPtr<FRHIPixelShader> FD3D12DynamicRHI::RHICreatePixelShader(const TArray<uint8>& Code, const FShaderBounds& Bounds)
{
	FD3D12PixelShader* Result = CreateStandartShader<FD3D12PixelShader>(Code);
	Result->ResourceCounts = Bounds;
	return Result;
}

TRefCountPtr<FRHITexture> FD3D12DynamicRHI::RHICreateTexture(const FRHITextureCreateDesc& CreateDesc)
{
	return CreateD3D12Texture(CreateDesc, GetAdapter().GetDevice());
}

void* FD3D12DynamicRHI::RHILockTexture2D(FRHITexture* TextureRHI, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithinMiptail, uint64* OutLockedByteCount)
{
	check(TextureRHI);
	FD3D12Texture* D3DTexture = static_cast<FD3D12Texture*>(TextureRHI);
	return D3DTexture->Lock(nullptr, MipIndex, 0, LockMode, DestStride, OutLockedByteCount);
}

void FD3D12DynamicRHI::RHIUnlockTexture2D(FRHITexture* TextureRHI, uint32 MipIndex, bool bLockWithinMiptail)
{
	check(TextureRHI);
	FD3D12Texture* D3DTexture = static_cast<FD3D12Texture*>(TextureRHI);
	D3DTexture->Unlock(nullptr, MipIndex, 0);
}

FD3D12Texture* FD3D12DynamicRHI::CreateEmptyD3D12Texture(const FRHITextureCreateDesc& CreateDesc, FD3D12Device* Device)
{
	return new FD3D12Texture(CreateDesc, Device);
}

FD3D12Texture* FD3D12DynamicRHI::CreateD3D12Texture(const FRHITextureCreateDesc& CreateDesc, FD3D12Device* Device)
{
	FD3D12Texture* NewTexture = CreateEmptyD3D12Texture(CreateDesc, Device);

	D3D12_HEAP_PROPERTIES TextureHeapProperties = {};
	TextureHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	
	const FLOAT Color[4] = { 0.0f, 0.f, 0.f, 1.f};

	D3D12_CLEAR_VALUE ClearValue = {};
	ClearValue.Format = GetDXGIFormat(CreateDesc.Format);
	for (uint32 i = 0; i < 4; ++i)
	{
		ClearValue.Color[i] = Color[i];
	}

	D3D12_RESOURCE_DESC TextureDesc = FD3D12Texture::GetResourceDescFromTextureDesc(CreateDesc);


	TRefCountPtr<ID3D12Resource> D3DTextureResource;

	DXCall(Device->GetDevice()->CreateCommittedResource(&TextureHeapProperties, D3D12_HEAP_FLAG_NONE, &TextureDesc,
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&D3DTextureResource)));

	FD3D12Resource* TextureResource = new FD3D12Resource(Device, D3DTextureResource.Get(),
		D3D12_RESOURCE_STATE_COMMON, TextureDesc);
	NewTexture->SetResource(TextureResource);
	DX12::SetName(TextureResource, CreateDesc.DebugName);

	NewTexture->CreateViews();


	return NewTexture;
}



IRHIComputeContext* FD3D12DynamicRHI::RHIGetCommandContext()
{
	return CreateCommandContext(GetAdapter().GetDevice(), ED3D12QueueType::Direct, false);
}

IRHICommandContext* FD3D12DynamicRHI::RHIGetDefaultContext()
{
	IRHICommandContext* DefaultCommandContext = nullptr;

	FD3D12Device* Device = GetAdapter().GetDevice();

	DefaultCommandContext = static_cast<IRHICommandContext*>(&Device->GetDefaultCommandContext());
	check(DefaultCommandContext);
	return DefaultCommandContext;
}




void FD3D12DynamicRHI::PostInit()
{
	FD3D12Adapter& Adapter = GetAdapter();
}

TRefCountPtr<FRHIGraphicsPipelineState> FD3D12DynamicRHI::RHICreateGraphicsPipelineState(const FGraphicsPipelineStateInitializer& Initalizer)
{
	FD3D12Device* Device = GetAdapter().GetDevice();
	FD3D12Adapter& Adapter = GetAdapter();
	const FD3D12RootSignature* RootSignature = Adapter.RootSignatureManager->GetRootSignature(Initalizer.BoundShaderState);
	
	FD3D12PipelineState* PipelineState = Device->GetPipelineStateManager().GetPipelineState(Initalizer, RootSignature);
	//check(PipelineState);


	return new FD3D12GraphicsPipelineState(Initalizer, RootSignature, PipelineState);
}