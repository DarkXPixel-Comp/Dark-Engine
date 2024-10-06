#include "Common.hlsl"

struct Params0
{
	double4x4 ViewMatrix;
	double4x4 ProjectionMatrix;
	double4x4 ViewProjectionMatrix;
};

struct Params1
{
	double4x4 ModelMatrix;
};


ConstantBuffer<Params0> Param0 : register(b0);
ConstantBuffer<Params1> Param1 : register(b1);


struct InVS
{
	float3 Position : Attribute0;
};

void VSMain(in float3 Position : Attribute0 , out float2 uv : TEXCOORD, out float4 InPosition : SV_POSITION)
{
	float4 PosW = mul(Param1.ModelMatrix, float4(Position, 1.f));
	float4x4 MVP = mul(Param0.ViewProjectionMatrix, Param1.ModelMatrix);
	float4 Pos = mul(MVP, float4(Position, 1.f));
	
	InPosition = float4(Pos);
		
	//InPosition = float4(In.Position, 1.f);
	uv = 0.f;
}

void PSMain(in float2 uv : TEXCOORD, in float4 InPosition : SV_POSITION, out float4 Color : SV_Target)
{
	Color = float4(cos(InPosition.x * InPosition.y), sin(InPosition.z), 1, 1);
	//Color = float4(Test, 1);
	//Color = float4(Test, 1.f);
}