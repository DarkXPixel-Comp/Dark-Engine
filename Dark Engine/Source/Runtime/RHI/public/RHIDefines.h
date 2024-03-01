#pragma once
#include "PixelFormat.h"

enum class ERHIInterfaceType
{
	Hidden,
	Null,
	D3D12
};


enum ERHIResourceType
{
	RRT_None,

	RRT_SamplerState,
	RRT_Buffer,
	RRT_Texture,
	RRT_Viewport,
	RRT_GraphicsPipelineState,
	RRT_ComputePipelineState,
	RRT_VertexShader,
	RRT_PixelShader,
	RRT_MeshShader
};

enum ERHIDescriptorHeapType
{
	Standart = 0,
	Sampler,
	RenderTarget,
	DepthStencil,
	Count
};



enum class ERHIAcces
{
	Unknown = 0,

	CpuRead = 1 << 0,
	Present = 1 << 1

};

enum EShaderType : uint8
{
	ST_Vertex = 0,
	ST_Mesh = 1,
	ST_Pixel,
	ST_Geometry,
	ST_Compute,
	ST_RayGen,
	ST_RayMiss,
	ST_RayHitGroup,
	ST_RayCallable
};

struct FRHIDescriptorHandle
{
	FRHIDescriptorHandle() = default;
	FRHIDescriptorHandle(ERHIDescriptorHeapType InType, uint32 InIndex):
		Index(InIndex),
		Type(InType) 
	{}


private:
	uint32 Index;
	uint32 Type;
};