#pragma once
#include "D3D12.h"


struct D3D12Light
{
	XMFLOAT3 Strenght;
	float FalloffStart;
	XMFLOAT3 Direction;
	float FalloffEnd;
	XMFLOAT3 Position;
	float SpotPower;
};



struct D3D12PassConstants
{
	D3D12Light Lights[16];
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectionMatrix;
	XMFLOAT4X4 ViewProjectionMatrix;
	XMFLOAT2 RenderTargetSize;
	float TotalTime;
	float DeltaTime;
	XMFLOAT3 EyePos;
	float NearZ;
	XMFLOAT4 AmbientLight;
	float FarZ;
};

struct D3D12MaterialConstants
{
	XMFLOAT4 Diffuse;
	XMFLOAT3 Frensel;
	float Roughness;
	XMFLOAT4X4 MatTransform;
};
