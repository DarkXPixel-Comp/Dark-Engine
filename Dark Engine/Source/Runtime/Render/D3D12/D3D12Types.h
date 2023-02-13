#pragma once
#include "D3D12.h"

struct D3D12PassConstants
{
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectionMatrix;
	XMFLOAT4X4 ViewProjectionMatrix;
	XMFLOAT4 Color = { 0.5, 0.5, 0.5, 1 };
};