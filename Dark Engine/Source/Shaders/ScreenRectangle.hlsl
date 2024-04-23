

void VSMain(in float4 Position : Attribute0, in float2 UV : Attribute1, out float4 InPosition : SV_POSITION, out float2 uv : TEXCOORD)
{
	InPosition = Position;
	uv = UV;
}

void PSMain(in float4 InPosition : SV_POSITION, in float2 uv : TEXCOORD, out float4 Color : SV_Target)
{
	Color = float4(InPosition.x * 0.001, 0, 0, 1);
	Color = float4(uv.xy, InPosition.x, 1);
}