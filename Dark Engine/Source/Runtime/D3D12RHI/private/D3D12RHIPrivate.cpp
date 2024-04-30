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
		//Element.AlignedByteOffset = Elements[i].Offset;
		Element.SemanticName = Elements[i].Name != TEXT("") ? !Elements[i].Name : "Attribute";
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

TRefCountPtr<FRHIRasterizerState> FD3D12DynamicRHI::RHICreateRasterizerState(const FRasterizerStateInitializer& Initializer)
{
	FD3D12RasterizerState* RasterizerState = new FD3D12RasterizerState();
	RasterizerState->Initializer = Initializer;

	D3D12_RASTERIZER_DESC& RasterizerDesc = RasterizerState->Desc;
	RasterizerDesc = {};
	RasterizerDesc.CullMode = TranslateCullMode(Initializer.CullMode);
	RasterizerDesc.FillMode = TranslateFillMode(Initializer.FillMode);
	RasterizerDesc.AntialiasedLineEnable = Initializer.bEnableLineAA;
	RasterizerDesc.MultisampleEnable = Initializer.bAllowMSAA;
	RasterizerDesc.DepthBias = Initializer.DepthBias;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = Initializer.DepthClipMode == RDCM_DepthClip;


	return RasterizerState;
}

TRefCountPtr<FRHIBuffer> FD3D12DynamicRHI::RHICreateBuffer(const FRHIBufferDesc& CreateDesc, const TCHAR* DebugName, ERHIAccess InitialState)
{
	return CreateD3D12Buffer(nullptr, CreateDesc, InitialState, DebugName);
}

void* FD3D12DynamicRHI::RHILockBuffer(FRHIBuffer* BufferRHI, uint32 Offset, uint32 Size, EResourceLockMode LockMode)
{
	check(BufferRHI);
	check(BufferRHI->GetSize() >= Size + Offset);

	FD3D12Buffer* Buffer = static_cast<FD3D12Buffer*>(BufferRHI);
	check(Buffer);

	FD3D12LockedResource& LockedData = Buffer->LockedData;
	FD3D12Adapter& Adapter = GetAdapter();
	FD3D12Device* Device = Adapter.GetDevice();

	void* Data = nullptr;
	FD3D12Resource* Resource = Buffer->ResourceLocation.GetResource();

	if (LockMode == RLM_ReadOnly)
	{
		LockedData.bLockedForReadOnly = true;
		TRefCountPtr<ID3D12Resource> NewResource;
		const D3D12_RESOURCE_DESC BufferDesc = CD3DX12_RESOURCE_DESC::Buffer(Size + Offset, D3D12_RESOURCE_FLAG_NONE);
		const D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);

		DXCall(Adapter.GetD3DDevice()->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
			&BufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&NewResource)));

		/*FD3D12Resource* StagingBuffer = new FD3D12Resource(Device, NewResource.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, BufferDesc, nullptr, D3D12_HEAP_TYPE_UPLOAD);*/
		FD3D12CommandContext& Context = Device->GetDefaultCommandContext();

		Context.TransitionResource(Resource, Resource->GetCurrentState(), D3D12_RESOURCE_STATE_COPY_SOURCE, 0);

		Context.GetCommandList().GetGraphicsCommandList()->CopyBufferRegion(NewResource.Get(), 0,
			Resource->GetResource(), Offset, Size);

		Context.FlushCommands();


		LockedData.Resource = NewResource;

		D3D12_RANGE Range{Offset, Size};
		NewResource->Map(0, &Range, &LockedData.MappedAddress);
		Data = LockedData.MappedAddress;
	}
	else
	{
		TRefCountPtr<ID3D12Resource> NewResource;
		const D3D12_RESOURCE_DESC BufferDesc = CD3DX12_RESOURCE_DESC::Buffer(Size, D3D12_RESOURCE_FLAG_NONE);
		const D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		DXCall(Adapter.GetD3DDevice()->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
			&BufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&NewResource)));

		LockedData.Resource = NewResource;
		
		D3D12_RANGE Range{ 0, Size };
		NewResource->Map(0, &Range, &LockedData.MappedAddress);
		Data = LockedData.MappedAddress;
	}


	LockedData.LockedOffset = Offset;
	LockedData.LockedPitch = Size;
	LockedData.bLocked = true;
	LockedData.bHasNeverBeenLocked = false;
	check(Data != nullptr);

	return Data;
}

void FD3D12DynamicRHI::RHIUnlockBuffer(FRHIBuffer* BufferRHI)
{
	FD3D12Buffer* Buffer = static_cast<FD3D12Buffer*>(BufferRHI);
	FD3D12LockedResource& LockedData = Buffer->LockedData;

	FD3D12Adapter& Adapter = GetAdapter();
	FD3D12Device* Device = Adapter.GetDevice();

	check(LockedData.bLocked);

	if (!LockedData.bLockedForReadOnly)
	{
		D3D12_RANGE Range{ 0, LockedData.LockedPitch };
		LockedData.Resource->Unmap(0, &Range);
		FD3D12CommandContext& Context = Device->GetDefaultCommandContext();

		D3D12_RESOURCE_STATES CurrentState = Buffer->ResourceLocation.GetResource()->GetCurrentState();

		Context.TransitionResource(LockedData.Resource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_COPY_SOURCE, 0);
		Context.TransitionResource(Buffer->ResourceLocation.GetResource(), 
			CurrentState, D3D12_RESOURCE_STATE_COPY_DEST, 0);


		Context.GetCommandList().GetGraphicsCommandList()->CopyBufferRegion(Buffer->ResourceLocation.GetResource()->GetResource(),
			LockedData.LockedOffset,
			LockedData.Resource.Get(), 0, LockedData.LockedPitch);

		Context.TransitionResource(Buffer->ResourceLocation.GetResource(),
			D3D12_RESOURCE_STATE_COPY_DEST, CurrentState, 0);

		Context.FlushCommands();
	}

	LockedData.Reset();

}

TRefCountPtr<FRHIUniformBuffer> FD3D12DynamicRHI::RHICreateUniformBuffer(const void* Contents, uint32 Size,EUniformBufferUsage Usage)
{
	FD3D12UniformBuffer* Result = new FD3D12UniformBuffer(GetAdapter().GetDevice(), Usage);


	if (Size > 0)
	{
		if (Contents)
		{
			void* MappedData = nullptr;
			TRefCountPtr<ID3D12Resource> NewResource;
			const D3D12_RESOURCE_DESC BufferDesc = CD3DX12_RESOURCE_DESC::Buffer(Size, D3D12_RESOURCE_FLAG_NONE);
			const D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			DXCall(GetAdapter().GetD3DDevice()->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
				&BufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&NewResource)));
			Result->ResourceLocation.AsStandAlone(new FD3D12Resource(GetAdapter().GetDevice(), NewResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, BufferDesc, nullptr, D3D12_HEAP_TYPE_UPLOAD), Size, &HeapProperties);
			MappedData = Result->ResourceLocation.GetMappedBaseAddress();

			FMemory::Memcpy(MappedData, Contents, Size);
		}
	}

	return Result;
}

void FD3D12DynamicRHI::RHIUpdateUniformBuffer(FRHIUniformBuffer* BufferRHI, const void* Contents, uint32 Size)
{
	FD3D12UniformBuffer* D3DBuffer = static_cast<FD3D12UniformBuffer*>(BufferRHI);

	if (Size > 0 && Contents)
	{
		if (Size > D3DBuffer->ResourceLocation.GetSize())
		{

		}
		else
		{
			void* MappedData = D3DBuffer->ResourceLocation.GetMappedBaseAddress();
			FMemory::Memcpy(MappedData, Contents, Size);
		}
	}
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

	bool bAllowClearValue = TextureDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER && (EnumHasAnyFlags(CreateDesc.Flags, ETextureCreateFlags::RenderTargetable) || EnumHasAnyFlags(CreateDesc.Flags, ETextureCreateFlags::DepthStencilTargetable));


	TRefCountPtr<ID3D12Resource> D3DTextureResource;

	DXCall(Device->GetDevice()->CreateCommittedResource(&TextureHeapProperties, D3D12_HEAP_FLAG_NONE, &TextureDesc,
		D3D12_RESOURCE_STATE_COMMON, bAllowClearValue ? &ClearValue : nullptr, IID_PPV_ARGS(&D3DTextureResource)));

	FD3D12Resource* TextureResource = new FD3D12Resource(Device, D3DTextureResource.Get(),
		D3D12_RESOURCE_STATE_COMMON, TextureDesc);
	NewTexture->SetResource(TextureResource);
	DX12::SetName(TextureResource, CreateDesc.DebugName);

	NewTexture->CreateViews();


	return NewTexture;
}

FD3D12Buffer* FD3D12DynamicRHI::CreateD3D12Buffer(FRHICommandListBase* RHICmdList, const FRHIBufferDesc& BufferDesc, ERHIAccess ResourceState, const TCHAR* DebugName)
{
	FD3D12Buffer* Buffer = nullptr;
	D3D12_RESOURCE_DESC Desc = CD3DX12_RESOURCE_DESC::Buffer(BufferDesc.Size);
	uint32 Alignment = 0;
	D3D12_RESOURCE_STATES InitialState = GetD3D12ResourceState(ResourceState);
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	FD3D12Device* Device = GetAdapter().GetDevice();
	FD3D12Resource* Resource = nullptr;
	TRefCountPtr<ID3D12Resource> D3DResource;

	if (BufferDesc.Size > 0)
	{
		DXCall(Device->GetDevice()->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE, &Desc, InitialState, nullptr, IID_PPV_ARGS(&D3DResource)));
		Resource = new FD3D12Resource(Device, D3DResource.Get(), InitialState, Desc);
		Buffer = new FD3D12Buffer(Device, BufferDesc);
		Buffer->ResourceLocation.SetResource(Resource);
		DX12::SetName(Resource, DebugName);
	}

	return Buffer;
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

TRefCountPtr<FRHIGraphicsPipelineState> FD3D12DynamicRHI::RHICreateGraphicsPipelineState(const FGraphicsPipelineStateInitializer& Initializer)
{
	auto It = GraphicsPipelineStateCache.find(Initializer);
	if (It != GraphicsPipelineStateCache.end())
	{
		return It->second;
	}

	FD3D12Device* Device = GetAdapter().GetDevice();
	FD3D12Adapter& Adapter = GetAdapter();
	const FD3D12RootSignature* RootSignature = Adapter.RootSignatureManager->GetRootSignature(Initializer.BoundShaderState);
	FD3D12PipelineState* PipelineState = Device->GetPipelineStateManager().GetPipelineState(Initializer, RootSignature);

	FD3D12GraphicsPipelineState* GraphicsPipelineState = new FD3D12GraphicsPipelineState(Initializer, RootSignature, PipelineState);
	GraphicsPipelineStateCache.emplace(Initializer, GraphicsPipelineState);



	return GraphicsPipelineState;
}												   