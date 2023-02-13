cbuffer cbObject : register(b0)
{
	float4x4 ModelMatrix;
}




cbuffer cbPass : register(b1)
{
	float4x4 gView;
	float4x4 gProj;
	float4x4 gViewProj;
	float4 gColor;
}





struct PSInput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};


PSInput main(float3 position : SV_Position,float3 normal : NORMAL, float4 color : COLOR, float2 uv : TEXCOORD)
{
    PSInput result;
	
	
	float4 posW = mul(ModelMatrix, float4(position, 1.f));
	result.position = mul(gViewProj, posW);
	result.color = (gColor);
    result.uv = uv;
    result.normal = normal;

    return result;
}

