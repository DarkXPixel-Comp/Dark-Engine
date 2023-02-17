#pragma once
#include "D3D12.h"


class D3D12Material
{
public:
	XMFLOAT4 m_diffuseAlbedo;
	XMFLOAT3 m_frenselR0;
	float m_roughness;
	XMFLOAT4X4 m_matTransform;



};