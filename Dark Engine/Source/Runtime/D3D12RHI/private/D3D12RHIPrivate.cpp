#include "D3D12RHIPrivate.h"
#include <Misc/AssertionMacros.h>
#include "RHICommandList.h"
#include "D3D12CommandContext.h"
#include "imgui_impl_dx12.h"
#include <D3D12Util.h>
#include "D3D12View.h"
#include "CoreGlobals.h"

#include "DDSTextureLoader.h"
#include "ResourceUploadBatch.h"
#include "Misc/Paths.h"
#include "D3D12Shader.h"
#include "D3D12RootSignature.h"
#include "D3D12PipelineState.h"

FD3D12DynamicRHI* FD3D12DynamicRHI::SingleD3D12RHI = nullptr;

FD3D12DynamicRHI::FD3D12DynamicRHI(const TArray<TSharedPtr<FD3D12Adapter>>& InChosenAdapters, bool InbUseNvStreamline)
	: ChosenAdapters(InChosenAdapters),
	bUseNvStreamline(InbUseNvStreamline)
{
	if (SingleD3D12RHI)
	{
		return;
	}

	SingleD3D12RHI = this;

	FeatureLevel = GetAdapter().GetFeatureLevel();

	GGlobalConsole.RegisterConsoleVariable(TEXT("r.D3D12.DebugMessages"), false, TEXT("Add callback message d3d12 debug"));
}

FD3D12DynamicRHI::~FD3D12DynamicRHI()
{

}

void FD3D12DynamicRHI::GetStaticsMemoryToFString(FString& OutString)
{
	TCHAR* Stats;

	GetAdapter().GetMemoryAllocator()->BuildStatsString(&Stats, FALSE);
	OutString = Stats;

	GetAdapter().GetMemoryAllocator()->FreeStatsString(Stats);
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

	if (Adapter.bDestroyed)
	{
		RequestExit();
		return;
	}


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

TRefCountPtr<FRHIViewport> FD3D12DynamicRHI::RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen)
{
	FD3D12Viewport* RenderingViewport = new FD3D12Viewport(&GetAdapter(), HWND(WindowHandle), SizeX, SizeY,
		bIsFullscreen, EPixelFormat::PF_R8G8B8A8_UNORM);
	RenderingViewport->Init();

#ifdef IMGUI
	/*FD3D12BindlessDescriptorManager& DescriptorManager = GetAdapter().GetDevice()->GetBindlessDescriptorManager();
	ImGuiDescriptorHandle = DescriptorManager.Allocate(ERHIDescriptorHeapType::Standart);

	ImGui_ImplDX12_Init(GetAdapter().GetD3DDevice(), RenderingViewport->GetCountBackBuffers(),
		GetDXGIFormat(EPixelFormat::PF_R8G8B8A8_UNORM),
		DescriptorManager.GetHeap(ERHIDescriptorHeapType::Standart)->GetHeap(),
		DescriptorManager.GetHeap(ERHIDescriptorHeapType::Standart)->GetCPUSlotHandle(ImGuiDescriptorHandle.GetIndex()),
		DescriptorManager.GetGpuHandle(ImGuiDescriptorHandle));*/
#endif

	return RenderingViewport;
}

void FD3D12DynamicRHI::GetAvailableDynamicSuperResolutions(TArray<FRHIDynamicSuperResolution>& OutAvaliable)
{
	const auto& Descs = GetAdapter().GetDSRDescs();
	uint32 Counter = 0;

	for (const auto& i : Descs)
	{
		FRHIDynamicSuperResolution Desc = {};
		Desc.Id = Counter++;
		Desc.Name = i.VariantName;
		Desc.OptimalFormat = GetPixelFormat(i.OptimalTargetFormat);
		OutAvaliable.Add(Desc);
	}
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


TRefCountPtr<FRHIComputeShader> FD3D12DynamicRHI::RHICreateComputeShader(const TArray<uint8>& Code, const FShaderBounds& Bounds)
{
	FD3D12ComputeShader* Result = CreateStandartShader<FD3D12ComputeShader>(Code);
	if (Result)
	{
		Result->ResourceCounts = Bounds;
		Result->RootSignature = GetAdapter().RootSignatureManager->GetRootSignature(Result);
	}
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
		//Element.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		Element.AlignedByteOffset = Elements[i].Offset == 0xff ? D3D12_APPEND_ALIGNED_ELEMENT : Elements[i].Offset;
		Element.SemanticName = Elements[i].Name != TEXT("") ? !Elements[i].Name : "Attribute";
	//	Element.InputSlot = Elements[i].bUseInstanceIndex ? 1 : 0;
		Element.InputSlot = Elements[i].StreamIndex;
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
	uint64 Hash = 0;
	hash_combine(Hash, Initializer);
	auto It = RasterizerStatesCache.find(Hash);

	FD3D12RasterizerState* RasterizerState;

	if (It != RasterizerStatesCache.end())
	{
		RasterizerState = It->second.Get();
		return RasterizerState;
	}
	else
	{
		RasterizerState = new FD3D12RasterizerState();
		RasterizerState->Initializer = Initializer;
		RasterizerStatesCache.emplace(Hash, RasterizerState);
	}

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



TRefCountPtr<FRHIUniformBuffer> FD3D12DynamicRHI::RHICreateUniformBuffer(const void* Contents, uint32 Size,EUniformBufferUsage Usage)
{
	FD3D12UniformBuffer* Result = new FD3D12UniformBuffer(GetAdapter().GetDevice(), Usage);
	FD3D12Adapter& Adapter = GetAdapter();

	if (Size > 0)
	{
		if (Contents)
		{
			void* MappedData = nullptr;
			TRefCountPtr<ID3D12Resource> NewResource;
			TRefCountPtr<D3D12MA::Allocation> NewAllocation;
			const D3D12_RESOURCE_DESC BufferDesc = CD3DX12_RESOURCE_DESC::Buffer(Size);
			const D3D12_RESOURCE_DESC1 BufferDesc1 = CD3DX12_RESOURCE_DESC1::Buffer(Size);
			const D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			D3D12MA::ALLOCATION_DESC AllocationDesc = {};
			AllocationDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
			/*DXCall(GetAdapter().GetD3DDevice10()->CreateCommittedResource3(&HeapProperties, D3D12_HEAP_FLAG_NONE,
				&BufferDesc1, D3D12_BARRIER_LAYOUT_UNDEFINED, nullptr, nullptr, 0, nullptr, IID_PPV_ARGS(&NewResource)));*/
			DXCall(GetAdapter().MemoryAllocator->CreateResource3(&AllocationDesc, &BufferDesc1, D3D12_BARRIER_LAYOUT_UNDEFINED, nullptr, 0, nullptr,&NewAllocation, IID_NULL, NULL));

			Result->ResourceLocation.AsStandAlone(new FD3D12Resource(
				Adapter.GetDevice(), NewAllocation.Get(), BufferDesc1, D3D12_HEAP_TYPE_UPLOAD), Size, &HeapProperties);



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
	D3D12_RESOURCE_DESC1 TextureDesc1 = FD3D12Texture::GetResourceDescFromTextureDesc1(CreateDesc);

	bool bAllowClearValue = TextureDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER && (EnumHasAnyFlags(CreateDesc.Flags, ETextureCreateFlags::RenderTargetable) || EnumHasAnyFlags(CreateDesc.Flags, ETextureCreateFlags::DepthStencilTargetable));


	TRefCountPtr<ID3D12Resource> D3DTextureResource;
	TRefCountPtr<D3D12MA::Allocation> TextureAllocation;

	D3D12MA::ALLOCATION_DESC AllocationDesc = {};
	AllocationDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

	/*DXCall(Device->GetDevice10()->CreateCommittedResource3(&TextureHeapProperties, D3D12_HEAP_FLAG_NONE, &TextureDesc1,
		D3D12_BARRIER_LAYOUT_COMMON, bAllowClearValue ? &ClearValue : nullptr, nullptr, 0, nullptr,IID_PPV_ARGS(&D3DTextureResource)));*/
	DXCall(GetAdapter().MemoryAllocator->CreateResource3(&AllocationDesc, &TextureDesc1,
		D3D12_BARRIER_LAYOUT_COMMON, bAllowClearValue ? &ClearValue : nullptr, 0, nullptr,
		&TextureAllocation, IID_NULL, NULL));


	/*FD3D12Resource* TextureResource = new FD3D12Resource(Device, D3DTextureResource.Get(),
		D3D12_RESOURCE_STATE_COMMON, TextureDesc);*/

	/*new FD3D12Resource(
		Adapter.GetDevice(), NewAllocation.Get(), BufferDesc1, D3D12_HEAP_TYPE_UPLOAD)*/

	FD3D12Resource* TextureResource = new FD3D12Resource(Device, TextureAllocation.Get(), TextureDesc1, D3D12_HEAP_TYPE_DEFAULT);

	NewTexture->SetResource(TextureResource);
	DX12::SetName(TextureResource, CreateDesc.DebugName);

	NewTexture->CreateViews();
	NewTexture->SetBarrierLayout(D3D12_BARRIER_LAYOUT_COMMON);


	return NewTexture;
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
		const D3D12_RESOURCE_DESC1 BufferDesc1 = CD3DX12_RESOURCE_DESC1::Buffer(Size + Offset);
		const D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
		D3D12MA::ALLOCATION_DESC AllocationDesc = {};
		AllocationDesc.HeapType = D3D12_HEAP_TYPE_READBACK;
		TRefCountPtr<D3D12MA::Allocation> NewAllocation;

		DXCall(Adapter.MemoryAllocator->CreateResource3(&AllocationDesc, &BufferDesc1, D3D12_BARRIER_LAYOUT_UNDEFINED,
			nullptr, 0, nullptr, &NewAllocation, IID_NULL, NULL));


		/*DXCall(Adapter.GetD3DDevice()->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
			&BufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&NewResource)));*/

		/*FD3D12Resource* StagingBuffer = new FD3D12Resource(Device, NewResource.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, BufferDesc, nullptr, D3D12_HEAP_TYPE_UPLOAD);*/
		FD3D12CommandContext& Context = Device->GetDefaultCommandContext();

		//Context.TransitionResource(Resource, Resource->GetCurrentState(), D3D12_RESOURCE_STATE_COPY_SOURCE, 0);
		Context.TransitionBuffer(Resource, D3D12_BARRIER_SYNC_COPY, D3D12_BARRIER_ACCESS_COPY_SOURCE);

		Context.FlushBarriers();
		Context.GetCommandList().GetGraphicsCommandList()->CopyBufferRegion(NewResource.Get(), 0,
			Resource->GetResource(), Offset, Size);

		Context.FlushCommands();


		LockedData.Allocation = NewAllocation;

		D3D12_RANGE Range{ Offset, Size };
		NewAllocation->GetResource()->Map(0, &Range, &LockedData.MappedAddress);
		Data = LockedData.MappedAddress;
	}
	else
	{
		//TRefCountPtr<ID3D12Resource> NewResource;
		TRefCountPtr<D3D12MA::Allocation> NewAllocation;
		const D3D12_RESOURCE_DESC BufferDesc = CD3DX12_RESOURCE_DESC::Buffer(Size, D3D12_RESOURCE_FLAG_NONE);
		const D3D12_RESOURCE_DESC1 BufferDesc1 = CD3DX12_RESOURCE_DESC1::Buffer(Size);
		const D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12MA::ALLOCATION_DESC AllocationDesc = {};
		AllocationDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
		DXCall(Adapter.MemoryAllocator->CreateResource3(&AllocationDesc, &BufferDesc1, D3D12_BARRIER_LAYOUT_UNDEFINED,
			nullptr, 0, nullptr, &NewAllocation, IID_NULL, NULL));

		/*DXCall(Adapter.GetD3DDevice10()->CreateCommittedResource3(&HeapProperties, D3D12_HEAP_FLAG_NONE,
			&BufferDesc1, D3D12_BARRIER_LAYOUT_UNDEFINED, nullptr, nullptr, 0, nullptr, IID_PPV_ARGS(&NewResource)));*/

		LockedData.Allocation = NewAllocation;

		D3D12_RANGE Range{ 0, Size };
		NewAllocation->GetResource()->Map(0, &Range, &LockedData.MappedAddress);
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
		LockedData.Allocation->GetResource()->Unmap(0, &Range);
		FD3D12CommandContext& Context = Device->GetDefaultCommandContext();

		//D3D12_RESOURCE_STATES CurrentState = Buffer->ResourceLocation.GetResource()->GetCurrentState();

		/*Context.TransitionResource(LockedData.Resource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_COPY_SOURCE, 0);*/
		//Context.TransitionResource(Buffer->ResourceLocation.GetResource(), D3D12_RESOURCE_STATE_COPY_DEST, 0);

		/*Context.TransitionResource(Buffer, CD3DX12_BUFFER_BARRIER(Buffer->GetBarrierSync(), D3D12_BARRIER_SYNC_COPY,
			Buffer->GetBarrierAccess(), D3D12_BARRIER_ACCESS_COPY_DEST, nullptr));*/

		Context.TransitionResource(Buffer, D3D12_BARRIER_SYNC_COPY, D3D12_BARRIER_ACCESS_COPY_DEST);

		Context.TransitionResource((FD3D12Buffer*)nullptr, CD3DX12_BUFFER_BARRIER(D3D12_BARRIER_SYNC_NONE,
			D3D12_BARRIER_SYNC_COPY, D3D12_BARRIER_ACCESS_NO_ACCESS, D3D12_BARRIER_ACCESS_COPY_SOURCE,
			LockedData.Allocation->GetResource()));

		Context.FlushBarriers();

		Context.GetCommandList().GetGraphicsCommandList()->CopyBufferRegion(Buffer->ResourceLocation.GetResource()->GetResource(),
			LockedData.LockedOffset,
			LockedData.Allocation->GetResource(), 0, LockedData.LockedPitch);


		/*Buffer->SetBarrierSync(D3D12_BARRIER_SYNC_COPY);
		Buffer->SetBarrierAccess(D3D12_BARRIER_ACCESS_COPY_DEST);*/
		Context.AddLockedResource(LockedData);
	}

	LockedData.Reset();

}

FD3D12Buffer* FD3D12DynamicRHI::CreateD3D12Buffer(FRHICommandListBase* RHICmdList, const FRHIBufferDesc& BufferDesc, ERHIAccess ResourceState, const TCHAR* DebugName)
{
	FD3D12Buffer* Buffer = nullptr;
	D3D12_RESOURCE_DESC Desc = CD3DX12_RESOURCE_DESC::Buffer(BufferDesc.Size);
	D3D12_RESOURCE_DESC1 Desc1 = CD3DX12_RESOURCE_DESC1::Buffer(BufferDesc.Size);
	uint32 Alignment = 0;
	D3D12_RESOURCE_STATES InitialState = GetD3D12ResourceState(ResourceState);
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	FD3D12Device* Device = GetAdapter().GetDevice();
	FD3D12Resource* Resource = nullptr;
	TRefCountPtr<ID3D12Resource> D3DResource;
	TRefCountPtr<D3D12MA::Allocation> Allocation;

	if (BufferDesc.Size > 0)
	{
		/*DXCall(Device->GetDevice10()->CreateCommittedResource3(
			&HeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&Desc1, D3D12_BARRIER_LAYOUT_UNDEFINED, nullptr, nullptr, 0, nullptr, IID_PPV_ARGS(&D3DResource)));*/

		D3D12MA::ALLOCATION_DESC AllocationDesc = {};
		AllocationDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

		DXCall(GetAdapter().MemoryAllocator->CreateResource3(&AllocationDesc, &Desc1, D3D12_BARRIER_LAYOUT_UNDEFINED, nullptr, 0, nullptr, &Allocation, IID_NULL, NULL));

		//Resource = new FD3D12Resource(Device, D3DResource.Get(), InitialState, Desc);
		Resource = new FD3D12Resource(Device, Allocation.Get(), Desc1, D3D12_HEAP_TYPE_DEFAULT);
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

TRefCountPtr<FRHIComputePipelineState> FD3D12DynamicRHI::RHICreateComputePipelineState(FRHIComputeShader* InComputeShader)
{
	check(InComputeShader);

	FD3D12ComputeShader* ComputeShader = static_cast<FD3D12ComputeShader*>(InComputeShader);
	const FD3D12RootSignature* RootSignature = ComputeShader->RootSignature;
	uint64 Hash = RootSignature->Hash;
	hash_without_hash_combine(Hash, ComputeShader->ShaderHash);
	
	auto It = ComputePipelineStateCache.find(Hash);
	if (It != ComputePipelineStateCache.end())
	{
		return It->second;
	}

	FD3D12Device* Device = GetAdapter().GetDevice();

	FD3D12PipelineState* PipelineState = Device->GetPipelineStateManager().GetPipelineState(ComputeShader, Hash);
	FD3D12ComputePipelineState* ComputePipelineState = new FD3D12ComputePipelineState(ComputeShader, RootSignature, PipelineState);
	ComputePipelineStateCache.emplace(Hash, ComputePipelineState);
	return ComputePipelineState;
}
