
struct BindlessParams
{
	uint ResultIndex;
};


ConstantBuffer<BindlessParams> Bindless : register(b0);

//RWTexture2D<float4> Result : register(u0);

[numthreads(4, 4, 1)]
void Main(uint3 ThreadId : SV_DispatchThreadID)
{
	RWTexture2D<float4> Result = ResourceDescriptorHeap[Bindless.ResultIndex];
	
	Result[ThreadId.xy] = float4(ThreadId.x, ThreadId.y, 0, 0);
}