#pragma once

#include "D3D12.h"

struct D3D12Camera
{
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;

	FLOAT m_fov;
	FLOAT m_aspectRatio;
public:
	D3D12Camera(XMFLOAT3 position = {}, XMFLOAT3 rotation = {});
	


};