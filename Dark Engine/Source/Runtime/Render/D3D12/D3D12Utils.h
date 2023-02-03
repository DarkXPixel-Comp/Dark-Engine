#pragma once
#include <unordered_map>
#include <memory>
#include <Windows.h>
#include "D3D12.h"




enum eShaderType
{
	Default = 0,
	Light,
	Size
};

class D3D12PipelineShaderRootSignature;


class D3DUtil
{
	static std::unordered_map<UINT, std::unique_ptr<D3D12PipelineShaderRootSignature>> Pipelines;


public:
	static UINT CalcConstantBufferByteSize(UINT byteSize)
	{
		return (byteSize + 255) & ~255;
	}
	static UINT GetCountPipelines() { return Pipelines.size(); }

	static void Init();
	static void InitPipelines()
	{
		for (size_t i = 0; i < eShaderType::Size; i++)
		{
			CreatePipeline(static_cast<eShaderType>(i));
		}
	}
	static D3D12PipelineShaderRootSignature* GetPipeline(UINT id) { return Pipelines.find(id)->second.get(); }
	static UINT CreatePipeline(eShaderType type);

	static UINT CreatePipeline(D3D12PipelineShaderRootSignature* PSO)
	{
		//Pipelines.emplace(PSO->GetID(), PSO);
	}

	static ID3D12Device8* GetDevice(); 
	static ID3D12CommandQueue* GetCommandQueue();

	static XMMATRIX CalcMVP()
	{
		XMMATRIX MVPmatrix = XMMatrixTranslation(0, 0, 3);
		XMVECTOR EyePos = XMVectorSet(0, 0, 0, 1);
		XMVECTOR FocucPos = XMVectorSet(0, 0, 3, 1);
		XMVECTOR UpDir = XMVectorSet(0, 1, 0, 0);

		MVPmatrix = XMMatrixMultiply(MVPmatrix, XMMatrixLookAtLH(EyePos, FocucPos, UpDir));
		MVPmatrix = XMMatrixMultiply(MVPmatrix, XMMatrixPerspectiveFovLH(90, 16 / 9, 0.1, 100));

		return MVPmatrix;

	}


};


