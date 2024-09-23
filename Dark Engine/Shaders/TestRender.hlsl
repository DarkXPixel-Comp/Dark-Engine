#include "Common.hlsl"


struct Parameters
{
};


//ConstantBuffer<Parameters> Params : register(b1);


struct InVS
{
	float3 Position : Attribute0;
};

void VSMain(InVS In, out float2 uv : TEXCOORD, out float4 InPosition : SV_POSITION)
{
	InPosition = float4(In.Position, 1.f);
	uv = 0.f;
}

void PSMain(in float2 uv : TEXCOORD, in float4 InPosition : SV_POSITION, out float4 Color : SV_Target)
{
	Color = float4(1, 1, 1, 1);
	//Color = float4(Test, 1);
	//Color = float4(Test, 1.f);
}