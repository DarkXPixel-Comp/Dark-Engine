#pragma once

#include "D3D12.h"

class D3D12Camera
{
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_direction;

	FLOAT m_fov;
	FLOAT m_aspectRatio;
public:
	D3D12Camera(XMFLOAT3 position, XMFLOAT3 rotation);


};