#include "D3D12Texture.h"
#include "D3D12View.h"

void FD3D12Texture::AddRTV(const D3D12_RENDER_TARGET_VIEW_DESC& RTVDesc, int32 OutIndex)
{
	RenderTargetView = MakeShareble<FD3D12RenderTargetView>(GetParentDevice());
	RenderTargetView->CreteView(RTVDesc, this);
}

void FD3D12Texture::SetResource(FD3D12Resource* InResource)
{
	Resource = MakeShareble(InResource);
}

