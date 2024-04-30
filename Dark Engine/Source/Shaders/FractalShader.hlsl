#include "Common.hlsl"


struct Parameters
{
	float2 Resolution;
	float Time;
};


ConstantBuffer<Parameters> Params : register(b0);


float3 Palette(float d)
{
	return lerp(float3(0.2f, 0.7f, 0.9f), float3(1.f, 0.f, 1.f), d);
}


float2 Rotate(float2 p, float Angle)
{
	float c = cos(Angle);
	float s = sin(Angle);
	return mul(p, float2x2(c, s, -s, c));
}

float Map(float3 P)
{
	for (int i = 0; i < 8; ++i)
	{
		float t = Params.Time * 0.2f;
		P.xz = Rotate(P.xz, t);
		P.xy = Rotate(P.xy, t * 1.89f);
		P.xz = abs(P.xz);
		P.xz -= 0.2;
	}
	return dot(sign(P), P) / 5.f;
}

float4 rm(float3 ro, float3 rd)
{
	float t = 0.f;
	float3 col = 0.f;
	float d;
	for (int i = 0.f; i < 64; ++i)
	{
		float3 p = ro + rd * t;
		d = Map(p) * 0.5f;
		if(d < 0.02f)
		{
			break;
		}
		if(d > 100.f)
		{
			break;
		}
		
		col += Palette(length(p) * 0.1f) / (400.f * d);
		t += d;
	}

	return float4(col, 1.f / (d * 100.f));
}




void VSMain(in float4 Position : Attribute0, in float2 UV : Attribute1,
				out float2 uv : TEXCOORD, out float4 InPosition : SV_POSITION)
{
	InPosition = Position;
	uv = UV;
}

void PSMain(in float2 uv : TEXCOORD, in float4 InPosition : SV_POSITION, out float4 Color : SV_TARGET)
{
	float2 newUv = (InPosition.xy - (Params.Resolution.xy / 2.f)) / Params.Resolution.x;
	
	//newUv = uv;
	//newUv.y = 1 - newUv.y;
	//newUv /= 2;
	
	//newUv = float2(uv.x, 1. - uv.y);
	//newUv = uv;
	
	float3 ro = float3(0.f, 0.f, -50.f);
	ro.xz = Rotate(ro.xz, Params.Time);
	float3 cf = normalize(-ro);
	float3 cs = normalize(cross(cf, float3(0.f, 1.f, 0.f)));
	float3 cu = normalize(cross(cf, cs));
	
	float3 uuv = ro + cf * 3.f + newUv.x * cs + newUv.y * cu;
	float3 rd = normalize(uuv - ro);
	
	float4 col = rm(ro, rd);
	
	Color = col;
}
