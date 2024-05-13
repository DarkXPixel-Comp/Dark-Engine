
RWTexture2D<float4> Result : register(u0);

[numthreads(1, 1, 1)]
void Main(uint3 ThreadId : SV_DispatchThreadID)
{
	Result[ThreadId.xy] = float4(1, 0, 0, 1);
}