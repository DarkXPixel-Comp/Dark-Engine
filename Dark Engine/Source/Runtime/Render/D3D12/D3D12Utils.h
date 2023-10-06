#pragma once
#include <unordered_map>
#include <memory>
#include <Containers/String/DarkString.h>
#include <Windows.h>
#include <Core/Memory/TUniquePtr.h>
#include <Core.h>
#include "D3D12.h"
#include "D3D12Camera.h"
#include "D3D12Texture.h"
#include "HAL/Platform.h"
#include "Containers/Array/Array.h"



#define D3D_DEFAULT_TEXTURE	"Resources/uv.dds"

enum eShaderType
{
	Default = 0,
	Light,
	Size
};

class D3D12PipelineShaderRootSignature;
class FD3D12Mesh;


class DENGINE_API D3DUtil
{
	static std::unordered_map<UINT, TUniquePtr<D3D12PipelineShaderRootSignature>> Pipelines;
	static std::unordered_map<FString, TUniquePtr<FD3D12Mesh>> m_meshes;
	static std::unordered_map<FString, TUniquePtr<D3D12Texture>> m_textures;
	static TArray<CD3DX12_STATIC_SAMPLER_DESC> m_samplers;


public:
	static UINT CalcConstantBufferByteSize(UINT byteSize)
	{
		return (byteSize + 255) & ~255;
	}
	
	static uint32 GetCountPipelines() { return Pipelines.size(); }
	static const TArray<CD3DX12_STATIC_SAMPLER_DESC> GetStaticSamples() { return m_samplers; }

	static void Init();
	static void Shutdown();
	static void InitPipelines()
	{
		for (uint32 i = 0; i < eShaderType::Size; i++)
		{
			CreatePipeline(static_cast<eShaderType>(i));
		}
	}
	static void InitStaticSamples();
	static D3D12PipelineShaderRootSignature* GetPipeline(UINT id) { return Pipelines.find(id)->second.get(); }
	static uint32 CreatePipeline(eShaderType type);

	static ID3D12Device8* GetDevice(); 
	static ID3D12CommandQueue* GetCommandQueue();
	static FD3D12Mesh* LoadMesh(FString path);
	static void DeleteMesh(FD3D12Mesh* mesh);
	static D3D12Texture* LoadTexture(FString path);
	static TArray<FD3D12Mesh*> LoadMeshes(FString path, bool bCombineMeshes);

	static XMMATRIX CalcMVP()
	{
		XMMATRIX MVPMatrix = XMMatrixTranslation(0, 0, 3);
		XMVECTOR EyePos = XMVectorSet(0, 0, 0, 1);
		XMVECTOR FocusPos = XMVectorSet(0, 0, 3, 1);
		XMVECTOR UpDir = XMVectorSet(0, 1, 0, 0);

		MVPMatrix = XMMatrixMultiply(MVPMatrix, XMMatrixLookAtLH(EyePos, FocusPos, UpDir));
		MVPMatrix = XMMatrixMultiply(MVPMatrix, XMMatrixPerspectiveFovLH(90, 16 / 9, 0.1, 1000));

		return MVPMatrix;

	}

	static float CalcFovY(float fov_x, float w, float h)
	{
		float a;
		float x;
		if (fov_x < 1)
			fov_x = 1;
		if (fov_x > 179)
			fov_x = 179;
		x = w / tan(fov_x / 360 * XM_PI);
		a = atan(h / x);
		a = a * 360 / XM_PI;
		return a;
	}

	static XMFLOAT4X4 GetViewProjMatrix(D3D12Camera* camera)
	{
		XMFLOAT4X4 ViewProj = {};
		if (!camera)
			return ViewProj;

		//camera->m_rotation.z = 130.f;

		XMFLOAT3 front;
		front.x = -(cos(XMConvertToRadians(camera->m_rotation.z)) * cos(XMConvertToRadians(camera->m_rotation.y)));
		front.y = -sin(XMConvertToRadians(camera->m_rotation.y));
		front.z = sin(XMConvertToRadians(camera->m_rotation.z))
			* cos(XMConvertToRadians(camera->m_rotation.y));

		const XMVECTOR position = XMLoadFloat3(&camera->m_position);
		const XMVECTOR direction = XMLoadFloat3(&front);
		XMMATRIX ViewMatrix = XMMatrixLookToLH(position, direction, XMVectorSet(0, 1, 0, 0));

		XMMATRIX ProjectionMatrix = XMMatrixPerspectiveFovLH(1.48, camera->m_aspectRatio,
			0.1f, 100.f);
		XMMATRIX VP = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);

		XMStoreFloat4x4(&ViewProj, VP);
		return ViewProj;
	}
	//static float GetDistanceBetweenPoints(XMFLOAT3 x1, XMFLOAT3 x2)
	//{
	//	XMVECTOR v1 = XMLoadFloat3(&x1);
	//}

};

static XMFLOAT3 operator-(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}


static XMFLOAT2 operator-(const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	XMFLOAT2 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;

	return result;
}


static XMFLOAT3 operator*(const XMFLOAT3& v1, const float f2)
{
	XMFLOAT3 result;
	result.x = v1.x * f2;
	result.y = v1.y * f2;
	result.z = v1.z * f2;

	return result;
}


