

float Scale;


float main(float2 uv : TEXCOORD) : SV_TARGET
{
    float x = sin(uv.x);
    return float4(1, x, 1, 1);
}