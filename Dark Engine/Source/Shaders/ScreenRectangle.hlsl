#include "Common.hlsl"
struct RenderResources
{
	uint TextureIndex;
};


struct T
{
	float Test;
};

ConstantBuffer<RenderResources> RenderResource : register(b0, space0);
ConstantBuffer<T> TestBuffer : register(b1);

//SamplerState DefaultSampler = GetGlobalSampler(Anisotropic16, Wrapped);





void VSMain(in float4 Position : Attribute0, in float2 UV : Attribute1, out float2 uv : TEXCOORD, out float4 InPosition : SV_POSITION)
{
	//StructuredBuffer<float3> PositionBuffer = ResourceDescriptorHeap[0];
	InPosition = Position;
	uv = UV;
}

void PSMain(in float2 uv : TEXCOORD, out float4 Color : SV_Target)
{
	//StructuredBuffer<float3> PositionBuffer = ResourceDescriptorHeap[0];
	//Color = float4(InPosition.x * 0.001, 0, 0, 1);
	Texture2D Texture = ResourceDescriptorHeap[RenderResource.TextureIndex];
	//float4 Col = Texture.Load(int3(0, 0, 0));
	//Color = float4(uv.xy, InPosition.x, 1);
	Color = float4(TestBuffer.Test, TestBuffer.Test, TestBuffer.Test, 1);
	Color = Texture.Sample(GetGlobalSampler(Anisotropic16, Wrapped), uv);
	//Color = float4(Col.x, 0, 0, 1);
	//Color = float4(1, 1, 1, 1);
	//Color = float4(Test, 1);
	//Color = float4(Test, 1.f);
}