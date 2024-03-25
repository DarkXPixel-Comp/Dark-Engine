#include "D3D12Texture.h"
#include "D3D12View.h"
#include "D3D12Util.h"


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
		Result.Format = DXGI_FORMAT_B8G8R8A8_TYPELESS;//GetDXGIFormat(InDesc.Format);
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

void* FD3D12Texture::GetNativeShaderResourceView() const
{
	//void* Result = (void*)(Parent->GetBindlessDescriptorManager().GetHeap(ERHIDescriptorHeapType::Standart)->GetGPUSlotHandle
	//(ShaderResourceView->GetCpuHandle().ptr)).ptr;

	void* Result = (void*)((Parent->GetBindlessDescriptorManager().
		GetGpuHandle(ShaderResourceView->GetBindlessHandle()))).ptr;
	return Result;
}

