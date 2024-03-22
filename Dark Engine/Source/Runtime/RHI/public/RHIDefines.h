#pragma once
#include "PixelFormat.h"
#include "CoreTypes.h"
#include "Misc/EnumClassFlags.h"

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

enum class ERHIDescriptorHeapType : uint8
{
	Standart = 0,
	Sampler,
	RenderTarget,
	DepthStencil,
	Count,
	Invalid = MAX_uint8
};

enum class ERHIBindlessConfiguration
{
	Disable,
	AllShaders,
	RayTracingShaders
};



enum class ERHIAcces
{
	Unknown = 0,

	CpuRead = 1 << 0,
	Present = 1 << 1,
	VertexOrIndexBuffer = 1 << 2,
	SRVGraphics = 1 << 3,
	UAVCompute = 1 << 4,
	UAVGraphics = 1 << 5,
	RTV = 1 << 6,
	CopyDesc = 1 << 7,
	DSVWrite = 1 << 8

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

enum class ETextureDimension
{
	Texture2D,
	Texture2DArray,
	Texture3D,
	TextureCube,
	TextureCubeArray
};

enum class ETextureCreateFlags : uint64
{
	None = 0,

	RenderTargetable = 1ull << 0,
	DepthStencilTargetable = 1ull << 1,
	ShaderResource = 1ull << 2,
	CPUWritable = 1ull << 3,
	Dynamic = 1ull << 4,
	UAV = 1ull << 5,
	Presentable = 1ull << 6,
	CPUReadBack = 1ull << 7,
};

ENUM_CLASS_FLAGS(ETextureCreateFlags);



struct FRHIDescriptorHandle
{
	FRHIDescriptorHandle() = default;
	FRHIDescriptorHandle(ERHIDescriptorHeapType InType, uint32 InIndex):
		Index(InIndex),
		Type((uint8)InType) 
	{}


	uint32 GetIndex() const { return Index; }
	ERHIDescriptorHeapType GetHeapType() const { return (ERHIDescriptorHeapType)Type; }

	FORCEINLINE bool IsValid() const { return Index != MAX_uint32 && Type != (uint8)ERHIDescriptorHeapType::Invalid; }

private:
	uint32 Index = MAX_uint32;
	uint8 Type = (uint8)ERHIDescriptorHeapType::Invalid;
};



