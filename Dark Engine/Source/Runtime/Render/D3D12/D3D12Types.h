#pragma once
#include "D3D12Main.h"


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


struct VertexDefault
{
	VertexDefault() = default;

	VertexDefault(const VertexDefault&) = default;
	VertexDefault& operator=(const VertexDefault&) = default;

	VertexDefault(VertexDefault&&) = default;
	VertexDefault& operator=(VertexDefault&&) = default;

	VertexDefault(
		XMFLOAT3 const& iposition,
		XMFLOAT3 const& inormal,
		XMFLOAT4 const& itextureCoordinate,
		XMFLOAT2 const& iuv,
		XMFLOAT3 const& itangent,
		XMFLOAT3 const& ibitangent
	) noexcept
	{
	}



	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 color;
	XMFLOAT3 tangent;
	XMFLOAT3 bitangent;
	XMFLOAT2 textureCoordinate;

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

private:
	static constexpr unsigned int InputElementCount = 6;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];


};
