#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <Windows.h>
#include "D3D12.h"
#include "D3D12Camera.h"





enum eShaderType
{
	Default = 0,
	Light,
	Size
};

class D3D12PipelineShaderRootSignature;
class D3D12Mesh;


class D3DUtil
{
	static std::unordered_map<UINT, std::unique_ptr<D3D12PipelineShaderRootSignature>> Pipelines;
	static std::unordered_map<std::string, std::unique_ptr<D3D12Mesh>> m_meshes;


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
	static D3D12Mesh* LoadMesh(std::string path);

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
	static XMFLOAT4X4 GetViewProjMatrix(D3D12Camera* camera)
	{
		XMFLOAT4X4 ViewProj = {};
		if (!camera)
			return ViewProj;


		XMFLOAT3 front;
		front.x = cos(XMConvertToRadians(camera->m_rotation.z))
			* cos(XMConvertToRadians(camera->m_rotation.y));
		front.y = sin(XMConvertToRadians(camera->m_rotation.y));
		front.z = sin(XMConvertToRadians(camera->m_rotation.z))
			* cos(XMConvertToRadians(camera->m_rotation.y));

		const XMVECTOR position = XMLoadFloat3(&camera->m_position);
		const XMVECTOR direction = XMLoadFloat3(&front);
		XMMATRIX ViewMatrix = XMMatrixLookToLH(position, direction, XMVectorSet(0, 1, 0, 0));
		XMMATRIX ProjectionMatrix = XMMatrixPerspectiveFovLH(camera->m_fov, camera->m_aspectRatio,
			0.1f, 100.f);
		XMMATRIX VP = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);

		XMStoreFloat4x4(&ViewProj, VP);
		return ViewProj;
	}
	static float GetDistanceBetweenPoints(XMFLOAT3 x1, XMFLOAT3 x2)
	{
		XMVECTOR v1 = XMLoadFloat3(&x1);




	}


};


