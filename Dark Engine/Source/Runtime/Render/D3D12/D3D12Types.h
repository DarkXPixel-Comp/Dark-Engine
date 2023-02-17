#pragma once
#include "D3D12.h"

struct D3D12PassConstants
{
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectionMatrix;
	XMFLOAT4X4 ViewProjectionMatrix;
	XMFLOAT2 RenderTargetSize;
	float TotalTime;
	float DeltaTime;
	XMFLOAT4 AmbientLight;
};

struct D3D12MaterialConstants
{
	XMFLOAT4 Diffuse;
	XMFLOAT3 Frensel;
	float Roughness;
	XMFLOAT4X4 MatTransform;
};