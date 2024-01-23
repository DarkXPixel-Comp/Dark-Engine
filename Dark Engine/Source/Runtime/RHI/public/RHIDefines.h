#pragma once

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
};

enum ERHIDescriptorHeapType
{
	Standart = 0,
	Sampler,
	RenderTarget,
	DepthStencil,
	Count
};