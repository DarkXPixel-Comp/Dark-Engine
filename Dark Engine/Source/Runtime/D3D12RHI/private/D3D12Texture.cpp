#include "D3D12Texture.h"
#include "D3D12View.h"
#include "D3D12Util.h"
#include "D3D12CommandContext.h"



D3D12_RESOURCE_DIMENSION GetD3D12Dimension(ETextureDimension InDimension)
{
	switch (InDimension)
	{
	case ETextureDimension::Texture2D: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	case ETextureDimension::Texture2DArray: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	case ETextureDimension::Texture3D: return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	case ETextureDimension::TextureCube: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	case ETextureDimension::TextureCubeArray: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	}

}



void FD3D12Texture::AddSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& SRVDesc)
{
	check(!ShaderResourceView);

	ShaderResourceView = MakeShareble<FD3D12ShaderResourceView>(GetParentDevice());
	ShaderResourceView->CreateView(SRVDesc, GetResource());
}

void FD3D12Texture::EmplaceRTV(D3D12_RENDER_TARGET_VIEW_DESC const& RTVDesc, int32 Index)
{
	RenderTargetViews[Index] = MakeShareble<FD3D12RenderTargetView>(GetParentDevice());
	RenderTargetViews[Index]->CreteView(RTVDesc, GetResource());
}

void FD3D12Texture::EmplaceSRV(D3D12_SHADER_RESOURCE_VIEW_DESC const& SRVDesc)
{
	check(!ShaderResourceView);

	ShaderResourceView = MakeShareble<FD3D12ShaderResourceView>(GetParentDevice());
	ShaderResourceView->CreateView(SRVDesc, GetResource());
}

void FD3D12Texture::EmplaceUAV(D3D12_UNORDERED_ACCESS_VIEW_DESC const& UAVDesc)
{
	check(!UnorderedAccessView);

	UnorderedAccessView = MakeShareble(new FD3D12UnorderedAccessView(GetParentDevice()));
	UnorderedAccessView->CreateView(UAVDesc, GetResource());
}


void FD3D12Texture::SetResource(FD3D12Resource* InResource)
{
	Resource = InResource;
}

void FD3D12Texture::CreateViews()
{
	D3D12_RESOURCE_DESC ResourceDesc = GetResource()->GetDesc();
	FRHITextureDesc Desc = GetDesc();

	const DXGI_FORMAT ResourceFormat = GetDXGIFormat(Desc.Format);
	const bool bTexture2D = Desc.IsTexture2D();
	const bool bTexture3D = Desc.IsTexture3D();
	const bool bTextureCube = Desc.IsTextureCube();
	const bool bTextureArray = Desc.IsTextureArray();

	bool bCreateRTV = EnumHasAnyFlags(ResourceDesc.Flags, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	bool bCreateDSV = EnumHasAnyFlags(ResourceDesc.Flags, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	bool bCreateShaderResource = !EnumHasAnyFlags(ResourceDesc.Flags, D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);
	bool bCreateUAV = EnumHasAnyFlags(ResourceDesc.Flags, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	FD3D12Device* Device = GetParentDevice();

	if (bCreateRTV)
	{
		if (bTexture3D)
		{
			D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
			RTVDesc.Format = ResourceFormat;
			RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
			RTVDesc.Texture3D.MipSlice = 0;
			RTVDesc.Texture3D.FirstWSlice = 0;
			RTVDesc.Texture3D.WSize = Desc.Depth;

			SetNumRTV(1);
			EmplaceRTV(RTVDesc, 0);
		}
		else
		{
			SetNumRTV(Desc.NumMips);

			uint32 RTVIndex = 0;

			for (uint32 MipIndex = 0; MipIndex < Desc.NumMips; ++MipIndex)
			{
				D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
				RTVDesc.Format = ResourceFormat;

				if (bTextureArray || bTextureCube)
				{
					RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
					RTVDesc.Texture2DArray.FirstArraySlice = 0;
					RTVDesc.Texture2DArray.ArraySize = ResourceDesc.DepthOrArraySize;
					RTVDesc.Texture2DArray.MipSlice = MipIndex;
					RTVDesc.Texture2DArray.PlaneSlice = 0;
				}
				else
				{
					RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
					RTVDesc.Texture2D.MipSlice = MipIndex;
					RTVDesc.Texture2D.PlaneSlice = 0;
				}

				EmplaceRTV(RTVDesc, RTVIndex++);
			}
		}
	}


	if (bCreateDSV)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
		DSVDesc.Format = ResourceFormat;
		if (bTextureArray || bTextureCube)
		{
			DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
			DSVDesc.Texture2DArray.FirstArraySlice = 0;
			DSVDesc.Texture2DArray.ArraySize = ResourceDesc.DepthOrArraySize;
			DSVDesc.Texture2DArray.MipSlice = 0;
		}
		else
		{
			DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			DSVDesc.Texture2D.MipSlice = 0;
		}

		//EmplaceDSV();
	}

	if (bCreateUAV)
	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};

		UAVDesc.Format = ResourceFormat;
		
		if (bTextureArray && bTexture2D)
		{
			UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
			UAVDesc.Texture2DArray.ArraySize = ResourceDesc.DepthOrArraySize;
			UAVDesc.Texture2DArray.FirstArraySlice = 0;
			UAVDesc.Texture2DArray.PlaneSlice = 0;
			UAVDesc.Texture2DArray.MipSlice = 0;
		}
		else if (bTexture2D)
		{
			UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			UAVDesc.Texture2D.MipSlice = 0;
			UAVDesc.Texture2D.PlaneSlice = 0;
		}
		else if (bTexture3D)
		{
			UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
			UAVDesc.Texture3D.FirstWSlice = 0;
			UAVDesc.Texture3D.WSize = -1;
			UAVDesc.Texture3D.MipSlice = 0;
		}
		EmplaceUAV(UAVDesc);
	}



	if (bCreateShaderResource)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

		SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		SRVDesc.Format = ResourceFormat;
		if (bTextureCube && bTextureArray)
		{
			SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
			SRVDesc.TextureCubeArray.MostDetailedMip = 0;
			SRVDesc.TextureCubeArray.MipLevels = Desc.NumMips;
			SRVDesc.TextureCubeArray.First2DArrayFace = 0;
			SRVDesc.TextureCubeArray.NumCubes = Desc.ArraySize;
		}
		else if(bTextureCube)
		{
			SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
			SRVDesc.TextureCube.MostDetailedMip = 0;
			SRVDesc.TextureCube.MipLevels = Desc.NumMips;
		}
		else if (bTextureArray)
		{
			SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			SRVDesc.Texture2DArray.MostDetailedMip = 0;
			SRVDesc.Texture2DArray.MipLevels = Desc.NumMips;
			SRVDesc.Texture2DArray.FirstArraySlice = 0;
			SRVDesc.Texture2DArray.ArraySize = ResourceDesc.DepthOrArraySize;
			SRVDesc.Texture2DArray.PlaneSlice = 0;
		}
		else if (bTexture3D)
		{
			SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
			SRVDesc.Texture3D.MipLevels = Desc.NumMips;
			SRVDesc.Texture3D.MostDetailedMip = 0;
		}
		else
		{
			SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = Desc.NumMips;
			SRVDesc.Texture2D.PlaneSlice = 0;
		}
		EmplaceSRV(SRVDesc);
	}




}

void FD3D12Texture::PrepareShaderResourceView()
{
	FD3D12CommandContext& Context = GetParentDevice()->GetDefaultCommandContext();
	//Context.TransitionResource(GetResource(), D3D12_RESOURCE_STATE_COMMON, 0);
	Context.TransitionResource(this, D3D12_BARRIER_SYNC_ALL_SHADING, D3D12_BARRIER_ACCESS_SHADER_RESOURCE, D3D12_BARRIER_LAYOUT_SHADER_RESOURCE);
}

FIntPoint FD3D12Texture::GetSize() const
{
	FIntPoint Result;

	if (GetResource())
	{
		auto ResourceDesc = GetResource()->GetResource()->GetDesc();
		Result.X = ResourceDesc.Width;
		Result.Y = ResourceDesc.Height;
	}


	return Result;
}

D3D12_RESOURCE_DESC FD3D12Texture::GetResourceDescFromTextureDesc(const FRHITextureCreateDesc& InDesc)
{
	D3D12_RESOURCE_DESC Result = {};

	bool bDenyShaderResource = false;

	if (InDesc.Dimension != ETextureDimension::Texture3D)
	{
		Result.Dimension = GetD3D12Dimension(InDesc.Dimension);
		Result.Alignment = 0;
		Result.Width = InDesc.Extent.X;
		Result.Height = InDesc.Extent.Y;
		Result.Format = GetDXGIFormat(InDesc.Format);
		Result.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		Result.MipLevels = InDesc.NumMips;
		Result.SampleDesc = { 1, 0 };
		Result.DepthOrArraySize = InDesc.ArraySize / (InDesc.IsTextureCube() ? 6 : 1);
		Result.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::RenderTargetable))
		{
			Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}
		else if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::DepthStencilTargetable))
		{
			Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}

		if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::DepthStencilTargetable) &&
			!EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::ShaderResource))
		{
			bDenyShaderResource = true;
		}

		if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::UAV))
		{
			Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		}
	}
	else
	{
		Result = CD3DX12_RESOURCE_DESC::Tex3D
		(
			GetDXGIFormat(InDesc.Format),
			InDesc.Extent.X,
			InDesc.Extent.Y,
			InDesc.Depth,
			InDesc.NumMips
		);

		if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::RenderTargetable))
		{
			Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}
	}

	if (bDenyShaderResource)
	{
		Result.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	}

	if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::UAV))
	{
		Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}



	return Result;
}

D3D12_RESOURCE_DESC1 FD3D12Texture::GetResourceDescFromTextureDesc1(const FRHITextureCreateDesc& InDesc)
{
	D3D12_RESOURCE_DESC1 Result = {};

	bool bDenyShaderResource = false;

	if (InDesc.Dimension != ETextureDimension::Texture3D)
	{
		Result.Dimension = GetD3D12Dimension(InDesc.Dimension);
		Result.Alignment = 0;
		Result.Width = InDesc.Extent.X;
		Result.Height = InDesc.Extent.Y;
		Result.Format = GetDXGIFormat(InDesc.Format);
		Result.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		Result.MipLevels = InDesc.NumMips;
		Result.SampleDesc = { 1, 0 };
		Result.DepthOrArraySize = InDesc.ArraySize / (InDesc.IsTextureCube() ? 6 : 1);
		Result.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::RenderTargetable))
		{
			Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}
		else if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::DepthStencilTargetable))
		{
			Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}

		if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::DepthStencilTargetable) &&
			!EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::ShaderResource))
		{
			bDenyShaderResource = true;
		}

		if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::UAV))
		{
			Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		}
	}
	else
	{
		Result = CD3DX12_RESOURCE_DESC1::Tex3D
		(
			GetDXGIFormat(InDesc.Format),
			InDesc.Extent.X,
			InDesc.Extent.Y,
			InDesc.Depth,
			InDesc.NumMips
		);

		if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::RenderTargetable))
		{
			Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}
	}

	if (bDenyShaderResource)
	{
		Result.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	}

	if (EnumHasAnyFlags(InDesc.Flags, ETextureCreateFlags::UAV))
	{
		Result.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}



	return Result;
}

void* FD3D12Texture::GetNativeShaderResourceView() const
{
	//void* Result = (void*)(Parent->GetBindlessDescriptorManager().GetHeap(ERHIDescriptorHeapType::Standart)->GetGPUSlotHandle
	//(ShaderResourceView->GetCpuHandle().ptr)).ptr;

	void* Result = (void*)((Parent->GetBindlessDescriptorManager().
		GetGpuHandle(ShaderResourceView->GetBindlessHandle()))).ptr;
	return Result;
}

FRHIDescriptorHandle FD3D12Texture::GetBindlessHandle() const
{
	return ShaderResourceView->GetBindlessHandle();
}

template <typename T>
FORCEINLINE constexpr T Align(T Val, uint64 Alignment)
{
	return (T)(((uint64)Val + Alignment - 1) & ~(Alignment - 1));
}


void* FD3D12Texture::Lock(FRHICommandListImmediate* RHICmdList, uint32 MipIndex, uint32 ArrayIndex, EResourceLockMode LockMode, uint32& DestStride, uint64* OutLockedByteCount)
{
	const static FString RHITextureLockName = TEXT("FRHITexture lock");

	FD3D12Device* Device = GetParentDevice();
	FD3D12Adapter* Adapter = Device->GetParentAdapter();

	const uint32 Subresource = CalcSubresource(MipIndex, ArrayIndex, this->GetDesc().NumMips);

	FD3D12LockedResource* LockedResource = new FD3D12LockedResource(Device);

	const int32 BlockSizeX = 1;
	const int32 BlockSizeY = 1;
	const int32 BlockBytes = 4;
	const int32 MipSizeX = FMath::Max(this->GetSize().X >> MipIndex, BlockSizeX);
	const int32 MipSizeY = FMath::Max(this->GetSize().Y >> MipIndex, BlockSizeY);
	const int32 NumBlockX = (MipSizeX + BlockSizeX - 1) / BlockSizeX;
	const int32 NumBlockY = (MipSizeY + BlockSizeY - 1) / BlockSizeY;

	const int32 XBytesAligned = Align(NumBlockX * BlockBytes, FD3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
	const int32 MipBytesAligned = XBytesAligned * NumBlockY;

	const D3D12_RESOURCE_DESC& CurrentResourceDesc = GetResource()->GetDesc();

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT PlacedTexture2D = {};
	Device->GetDevice()->GetCopyableFootprints(&CurrentResourceDesc, Subresource, 1, 0, &PlacedTexture2D, nullptr, nullptr, nullptr);

	PlacedTexture2D.Offset = 0;
	DestStride = PlacedTexture2D.Footprint.RowPitch;
	const uint64 SubresourceSize = PlacedTexture2D.Footprint.RowPitch * PlacedTexture2D.Footprint.Height * PlacedTexture2D.Footprint.Depth;
	LockedResource->Footprint = PlacedTexture2D.Footprint;


	if (OutLockedByteCount)
	{
		*OutLockedByteCount = SubresourceSize;
	}

	void* Data = nullptr;

	if (LockMode == RLM_WriteOnly)
	{
		const int32 BufferSize = Align(SubresourceSize, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
		const D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(BufferSize,
			D3D12_RESOURCE_FLAG_NONE);
		const D3D12_RESOURCE_DESC1 ResourceDesc1 = CD3DX12_RESOURCE_DESC1::Buffer(BufferSize);
		const D3D12_HEAP_PROPERTIES HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		const D3D12_RANGE Range = { (SIZE_T)0, (SIZE_T)BufferSize };

		Adapter->GetD3DDevice10()->CreateCommittedResource3(&HeapProps,
			D3D12_HEAP_FLAG_NONE, &ResourceDesc1,
			D3D12_BARRIER_LAYOUT_UNDEFINED, nullptr, nullptr, 0, nullptr,
			IID_PPV_ARGS(&LockedResource->Resource));

	

		LockedResource->Resource->Map(0, &Range, &Data);
		LockedResource->MappedAddress = Data;
	}
	else
	{
		LockedResource->bLockedForReadOnly = true;

		const D3D12_RESOURCE_DESC& TextureDesc = GetResource()->GetDesc();
		TRefCountPtr<ID3D12Resource> TextureResource;

		const D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(SubresourceSize,
			D3D12_RESOURCE_FLAG_NONE);

		const D3D12_RESOURCE_DESC1 ResourceDesc1 = CD3DX12_RESOURCE_DESC1::Buffer(SubresourceSize);

		const D3D12_HEAP_PROPERTIES HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);

		Adapter->GetD3DDevice10()->CreateCommittedResource3(&HeapProps, D3D12_HEAP_FLAG_NONE,
			&ResourceDesc1, D3D12_BARRIER_LAYOUT_UNDEFINED, nullptr, nullptr, 0, nullptr, IID_PPV_ARGS(&TextureResource));

		LockedResource->Resource = TextureResource;

		CD3DX12_TEXTURE_COPY_LOCATION DestCopyLocation(TextureResource.Get(), PlacedTexture2D);
		CD3DX12_TEXTURE_COPY_LOCATION SourceCopyLocation(GetResource()->GetResource(), Subresource);
		

		FD3D12CommandContext& Context = Device->GetDefaultCommandContext();
		//Context.TransitionResource(GetResource(), D3D12_RESOURCE_STATE_COPY_SOURCE, 0);


		/*Context.TransitionResource(this, CD3DX12_TEXTURE_BARRIER(
			GetBarrierSync(), D3D12_BARRIER_SYNC_COPY,
			GetBarrierAccess(), D3D12_BARRIER_ACCESS_COPY_SOURCE,
			GetBarrierLayout(), D3D12_BARRIER_LAYOUT_COPY_SOURCE, nullptr, CD3DX12_BARRIER_SUBRESOURCE_RANGE(Subresource)));*/

		Context.TransitionResource(this, D3D12_BARRIER_SYNC_COPY, D3D12_BARRIER_ACCESS_COPY_SOURCE, D3D12_BARRIER_LAYOUT_COPY_SOURCE);


		Context.FlushBarriers();
		//Context.FlushCommands();
		Context.GetCommandList().GetGraphicsCommandList()->CopyTextureRegion
		(
			&DestCopyLocation,
			0, 0, 0,
			&SourceCopyLocation,
			nullptr
		);


		Context.FlushCommands();
		LockedResource->LockedPitch = XBytesAligned;
		
		const D3D12_RANGE Range = { (SIZE_T)0, (SIZE_T)MipBytesAligned };
		LockedResource->Resource->Map(0, &Range, &Data);
	}
	


	LockedMap.insert(std::pair<uint32, FD3D12LockedResource*>(Subresource, LockedResource));

	return Data;
}

void FD3D12Texture::Unlock(FRHICommandListImmediate* RHICmdList, uint32 MipIndex, uint32 ArrayIndex)
{
	const uint32 Subresource = CalcSubresource(MipIndex, ArrayIndex, TextureDesc.NumMips);

	const int32 BlockSizeX = 1;
	const int32 BlockSizeY = 1;
	const int32 BlockBytes = 4;
	const int32 MipSizeX = FMath::Max(this->GetSize().X >> MipIndex, BlockSizeX);
	const int32 MipSizeY = FMath::Max(this->GetSize().Y >> MipIndex, BlockSizeY);
	const int32 NumBlockX = (MipSizeX + BlockSizeX - 1) / BlockSizeX;
	const int32 NumBlockY = (MipSizeY + BlockSizeY - 1) / BlockSizeY;
 
	FD3D12LockedResource* LockedResource = LockedMap[Subresource];
	check(LockedResource);

	if (!LockedResource->bLockedForReadOnly)
	{
		FD3D12CommandContext& DefaultContext = GetParentDevice()->GetDefaultCommandContext();
		FD3D12Resource* Resource = GetResource();
		
		const D3D12_RESOURCE_DESC& ResourceDesc = Resource->GetDesc();
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT PlacedTexture2D;
		PlacedTexture2D.Offset = 0;
		PlacedTexture2D.Footprint = LockedResource->Footprint;


		CD3DX12_TEXTURE_COPY_LOCATION SourceCopyLocation(LockedResource->Resource.Get(), PlacedTexture2D);

		UpdateTexture(Subresource, 0, 0, 0, SourceCopyLocation);

		DefaultContext.AddLockedResource(*LockedResource);
	}


	delete(LockedResource);
	LockedMap.erase(Subresource);
}

void FD3D12Texture::UpdateTexture(uint32 MipIndex, uint32 DestX, uint32 DestY, uint32 DestZ, const D3D12_TEXTURE_COPY_LOCATION& SourceCopyLocation)
{
	FD3D12CommandContext& DefaultContext = GetParentDevice()->GetDefaultCommandContext();

	D3D12_RESOURCE_STATES CurrentState = GetResource()->GetCurrentState();

	/*DefaultContext.TransitionResource(GetResource(), CurrentState,
		D3D12_RESOURCE_STATE_COPY_DEST, MipIndex);*/

	/*DefaultContext.TransitionResource(this, CD3DX12_TEXTURE_BARRIER(
		GetBarrierSync(), D3D12_BARRIER_SYNC_COPY,
		GetBarrierAccess(), D3D12_BARRIER_ACCESS_COPY_DEST,
		BarrierLayout, D3D12_BARRIER_LAYOUT_COPY_DEST, nullptr, CD3DX12_BARRIER_SUBRESOURCE_RANGE(MipIndex)));*/

	DefaultContext.TransitionResource(this, D3D12_BARRIER_SYNC_COPY,
		D3D12_BARRIER_ACCESS_COPY_DEST, D3D12_BARRIER_LAYOUT_COPY_DEST, CD3DX12_BARRIER_SUBRESOURCE_RANGE(MipIndex));

	DefaultContext.FlushBarriers();


	CD3DX12_TEXTURE_COPY_LOCATION DestCopyLocation(GetResource()->GetResource(), MipIndex);

	//DefaultContext.FlushCommands()

	DefaultContext.GetCommandList().GetGraphicsCommandList()->CopyTextureRegion(&DestCopyLocation,
		DestX, DestY, DestZ, &SourceCopyLocation, nullptr);


	/*DefaultContext.TransitionResource(this, CD3DX12_TEXTURE_BARRIER(
		GetBarrierSync(), D3D12_BARRIER_SYNC_DRAW,
		GetBarrierAccess(), D3D12_BARRIER_ACCESS_SHADER_RESOURCE,
		GetBarrierLayout(), D3D12_BARRIER_LAYOUT_SHADER_RESOURCE, nullptr, CD3DX12_BARRIER_SUBRESOURCE_RANGE(MipIndex)));*/

	DefaultContext.TransitionResource(this, D3D12_BARRIER_SYNC_DRAW, D3D12_BARRIER_ACCESS_SHADER_RESOURCE, D3D12_BARRIER_LAYOUT_SHADER_RESOURCE);

	/*SetBarrierSync(D3D12_BARRIER_SYNC_DRAW);
	SetBarrierAccess(D3D12_BARRIER_ACCESS_SHADER_RESOURCE);*/

	/*DefaultContext.TransitionResource(GetResource(), D3D12_RESOURCE_STATE_COPY_DEST,
		CurrentState, MipIndex);*/

	//DefaultContext.FlushCommands();
}


