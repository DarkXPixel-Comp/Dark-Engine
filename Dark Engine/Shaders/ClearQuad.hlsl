#include "Common.hlsl"


struct BindlessParameters
{
	uint TextureClearQuadIndex;
	
};


ConstantBuffer<BindlessParameters> Params : register(b0);



void ClearQuadVS(in float4 Position : Attribute0, in float2 UV : Attribute1, out float2 uv : TEXCOORD, out float4 InPosition : SV_POSITION)
{
	InPosition = Position;
	uv = UV;
}

void OneTexturePS(in float2 uv : TEXCOORD, in float4 InPosition : SV_POSITION, out float4 Color : SV_Target)
{
	Texture2D OneTexture = ResourceDescriptorHeap[Params.TextureClearQuadIndex];
	
	Color = OneTexture.Sample(GetGlobalSampler(Anisotropic16, Wrapped), uv);
}