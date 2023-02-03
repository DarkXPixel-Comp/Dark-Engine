#include "D3D12Utils.h"
#include "D3D12Render.h"
#include "D3D12PSO.h"
#include <Engine/public/DEngine.h>



std::unordered_map<UINT, std::unique_ptr<D3D12PipelineShaderRootSignature>> D3DUtil::Pipelines;

UINT D3DUtil::CreatePipeline(eShaderType type)
{
	auto render = static_cast<D3D12Renderer*>(GEngine.GetRenderer());

	switch (type)
	{
	case Default:
	{
		std::vector<CD3DX12_ROOT_PARAMETER1> parametrs(1);
		std::vector<D3D12_ROOT_PARAMETER1> pParametrs(parametrs.size());


		parametrs[0].InitAsConstantBufferView(0, 0);
		//parametrs[1].InitAsConstantBufferView(1, 0);

		for (size_t i = 0; i < parametrs.size(); i++) { pParametrs[i] = parametrs[i]; }

		D3D12PipelineShaderRootSignature* PSO = new D3D12PipelineShaderRootSignature(render->m_device.Get(),
			"shaders/VertexShader.hlsl", "shaders/PixelShader.hlsl", pParametrs);

		Pipelines.emplace(PSO->GetID(), PSO);
	}
	break;
	case Light:
		break;
	default:
		break;
	}

	return 0;
}


ID3D12Device8* D3DUtil::GetDevice()
{
	return static_cast<D3D12Renderer*>(DEngine::GetEngine()->GetRenderer())->m_device.Get();
}
ID3D12CommandQueue* D3DUtil::GetCommandQueue()
{
	return static_cast<D3D12Renderer*>(DEngine::GetEngine()->GetRenderer())->m_commandQueue.Get();
}

