cbuffer mvp : register(b0)
{
    matrix MVP;
}





struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 posObj : POSITION;

};



PSInput main(float3 position : SV_Position,float3 normal : NORMAL, float4 color : COLOR, float2 uv : TEXCOORD)
{
    PSInput result;



    result.position = mul(MVP, float4(position, 1.0f));
    result.color = (color);
    result.uv = uv;
    result.normal = normal;
    result.posObj = position;

    return result;
}

