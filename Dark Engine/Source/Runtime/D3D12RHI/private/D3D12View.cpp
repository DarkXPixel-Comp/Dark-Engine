#include "D3D12View.h"
#include "D3D12Texture.h"
#include "D3D12Device.h"


void FD3D12RenderTargetView::CreteView(const D3D12_RENDER_TARGET_VIEW_DESC& InDesc, FD3D12Resource* InResource)
{
	Resource = InResource;
	RTVDesc = InDesc;

	Parent->GetDevice()->CreateRenderTargetView(Resource->GetResource(), &InDesc, CpuHandle);

	UpdateBindlessSlot();
}


FD3D12View::~FD3D12View()
{
	GetParentDevice()->GetCpuDescriptorManger(HeapType).FreeHeapSlot(CpuHandle);
	if (BindlessHandle.IsValid())
	{
		GetParentDevice()->GetBindlessDescriptorManager().ImmediateFree(BindlessHandle);
		BindlessHandle = {};
	}
}

void FD3D12View::UpdateBindlessSlot()
{
	if (BindlessHandle.IsValid())
	{
		FD3D12BindlessDescriptorManager& BindlessManager = GetParentDevice()->GetBindlessDescriptorManager();
		BindlessManager.UpdateImmediate(BindlessHandle, CpuHandle);
	}
}

void FD3D12ShaderResourceView::CreateView(const D3D12_SHADER_RESOURCE_VIEW_DESC& InDesc, FD3D12Resource* InResource)
{
	Resource = InResource;
	SRVDesc = InDesc;

	Parent->GetDevice()->CreateShaderResourceView(Resource->GetResource(), &InDesc, CpuHandle);
	
	UpdateBindlessSlot();

	/*FD3D12BindlessDescriptorManager& BindlessManager = GetParentDevice()->GetBindlessDescriptorManager();
	TestGPUHandle = BindlessManager.GetGpuHandle(BindlessHandle);*/
}

void FD3D12UnorderedAccessView::CreateView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& InDesc, FD3D12Resource* InResource)
{
	Resource = InResource;
	UAVDesc = InDesc;

	Parent->GetDevice()->CreateUnorderedAccessView(Resource->GetResource(), nullptr, &UAVDesc, CpuHandle);

	UpdateBindlessSlot();
}
