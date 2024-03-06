#include "D3D12View.h"
#include "D3D12Texture.h"
#include "D3D12Device.h"


void FD3D12RenderTargetView::CreteView(const D3D12_RENDER_TARGET_VIEW_DESC& InDesc, FD3D12Texture* InResource)
{
	Resource = InResource;
	RTVDesc = InDesc;

	//Parent->GetDevice()->CreateRenderTargetView(Resource->GetResource()->GetResource(), &InDesc, );
}
