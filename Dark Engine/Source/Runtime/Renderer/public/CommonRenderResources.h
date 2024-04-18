#pragma once
#include "Math/MathFwd.h"
#include "RHIResources.h"
#include "DynamicRHI.h"


struct FFilterVertex
{
	FVector4f Position;
	FVector2f UV;

	FFilterVertex()
	{}

	FFilterVertex(FVector4f InPositon, FVector2f InUV) : 
		Position(InPositon), UV(InUV)
	{}
};


class FFilterVertexDeclaration
{
public:
	TRefCountPtr<FRHIVertexDeclaration>	VertexDeclaration;

	void Init()
	{
		FVertexDeclarationElementList Elements;
		uint16 Stride = sizeof(FFilterVertex);
		Elements.Add(FVertexElement(VET_Float4, 0, 0, 0, Stride));
		Elements.Add(FVertexElement(VET_Float2, 1, 0, 0, Stride));
		VertexDeclaration = RHICreateVertexDeclaration(Elements);
	}
};

class FScreenRectangleVertexBuffer
{
public:
	TRefCountPtr<FRHIBuffer> VertexBuffer;

	void Init()
	{
		if (VertexBuffer)
		{
			return;
		}
		TArray<FFilterVertex> Vertices(4);

		Vertices[0].Position = FVector4f(-1, -1, 0, 1);
		Vertices[0].UV = FVector2f(0, 1);

		Vertices[1].Position = FVector4f(-1, 1, 0, 1);
		Vertices[1].UV = FVector2f(0, 0);

		Vertices[2].Position = FVector4f(1, 1, 0, 1);
		Vertices[2].UV = FVector2f(1, 0);

		Vertices[3].Position = FVector4f(1, -1, 0, 1);
		Vertices[3].UV = FVector2f(1, 1);

	/*	Vertices[4].Position = FVector4f(-1, -1, 0, 1);
		Vertices[4].UV = FVector2f(0, 1);

		Vertices[5].Position = FVector4f(-1, -1, 0, 1);
		Vertices[5].UV = FVector2f(0, 1);*/


		VertexBuffer = RHICreateBuffer(FRHIBufferDesc(sizeof(FFilterVertex) * Vertices.Num(),
			sizeof(FFilterVertex)), TEXT("FScreenRectangleVertexBuffer"), ERHIAccess(ERHIAccess::VertexOrIndexBuffer));

		void* Data = RHILockBuffer(VertexBuffer.Get(), 0, sizeof(FFilterVertex) * Vertices.Num(), RLM_WriteOnly);
		FMemory::Memcpy(Data, Vertices.GetData(), sizeof(FFilterVertex) * Vertices.Num());

		RHIUnlockBuffer(VertexBuffer.Get());
	}
};