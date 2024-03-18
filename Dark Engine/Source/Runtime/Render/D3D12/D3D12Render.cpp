#include "D3D12Render.h"
#include <DEngine.h>
#include "D3D12Utils.h"
#include "D3D12Scene.h"
#include "D3D12PSO.h"
#include <World.h>
#include <Timer/GameTimer.h>
#include <ResourceUploadBatch.h>
#include <BufferHelpers.h>


#include <d3d12.h>



#undef max



void TransitionResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> commandList,
	Microsoft::WRL::ComPtr<ID3D12Resource> resource,
	D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		resource.Get(),
		beforeState, afterState);

	commandList->ResourceBarrier(1, &barrier);


	

}


D3D12Renderer::D3D12Renderer()
{
}

int32_t D3D12Renderer::Init()
{
	return 0;
}


void D3D12Renderer::OnInput(eInputActionState action)
{
}

void D3D12Renderer::Shutdown()
{
}



static DirectX::XMFLOAT3 SphericalToCartesian(float radius, float theta, float phi)
{
	return XMFLOAT3(-(radius * sinf(phi) * cosf(theta)),
		-(radius * cosf(phi)),
		-(radius * sinf(phi) * sinf(theta)));
}




void D3D12Renderer::BeginFrame(D3D12Scene* scene)
{
}


void D3D12Renderer::OnInterface()
{
}

void D3D12Renderer::SetResolution(int32 Width, int32 Height, bool IsFullScreen)
{
}

void D3D12Renderer::Render(D3D12Scene* scene)
{
}

void D3D12Renderer::RenderObj()
{
}


void D3D12Renderer::PostProcessing(D3D12_GPU_DESCRIPTOR_HANDLE renderHandle, ID3D12Resource* outputHandle)
{







}


void D3D12Renderer::RenderScene()
{
	


}



void D3D12Renderer::EndFrame()
{
}

void D3D12Renderer::OnResize(long x, long y)
{
	
}


void D3D12Renderer::Update()
{
	

}

uint64 D3D12Renderer::Signal(uint64 &fenceValue)
{
	return 0;
}

void D3D12Renderer::WaitForFenceValue(uint64 fenceValue)
{
}

void D3D12Renderer::WaitFrame()
{
}

