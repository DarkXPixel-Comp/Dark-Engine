#pragma once
#include "D3D12.h"
#include "D3D12Resource.h"
#include "D3D12Texture.h"


class D3D12Material
{
	//D3D12UploadBufferResource<>

public:
	XMFLOAT4 m_diffuseAlbedo;
	XMFLOAT3 m_frenselR0;
	float m_roughness;
	XMFLOAT4X4 m_matTransform;


	int64_t m_BaseDescriptorIndex = -1;
	
	D3D12Texture* t_Albedo;
	D3D12Texture* t_Roughness;
	D3D12Texture* t_Metallic;
	D3D12Texture* t_AmbientOcclusion;
	D3D12Texture* t_Normal;
	D3D12Texture* t_Specular;

};