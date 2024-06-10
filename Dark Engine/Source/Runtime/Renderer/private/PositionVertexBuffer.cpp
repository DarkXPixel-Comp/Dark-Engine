#include "PositionVertexBuffer.h"


void FPositionVertexBuffer::Init(uint32 InNumVertices, bool bInNeedCPUAccess)
{
	NumVertices = InNumVertices;
	bNeedCPUAccess = bInNeedCPUAccess;

	VertexData.Resize(NumVertices);
}


void FPositionVertexBuffer::Init(const TArray<FStaticMeshVertex>& InVertices, bool bNeedCPUAccess)
{
	Init(InVertices.Num(), bNeedCPUAccess);

	for (uint32 i = 0; i < InVertices.Num(); ++i)
	{
		VertexPosition(i) = InVertices[i].Position;
	}
}

void FPositionVertexBuffer::InitRHI(FRHICommandListImmediate& RHICmdList)
{
	VertexBufferRHI = RHICreateBuffer(FRHIBufferDesc(sizeof(FPositionVertex), Stride), TEXT("FPositionVertexBuffer"), ERHIAccess::VertexOrIndexBuffer);

	void* Data = RHILockBuffer(VertexBufferRHI.Get(), 0, VertexBufferRHI->GetSize(), RLM_WriteOnly);

	FMemory::Memcpy(Data, VertexData.GetData(), VertexBufferRHI->GetSize());
	RHIUnlockBuffer(VertexBufferRHI.Get());
}
