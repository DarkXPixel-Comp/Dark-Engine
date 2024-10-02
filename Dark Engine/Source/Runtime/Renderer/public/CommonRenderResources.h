#pragma once
#include "RHIResources.h"
#include "DynamicRHI.h"
#include "RenderResource.h"
#include "GlobalResource.h"
#include "Math/Vector4.h"
#include "Math/Vector2D.h"


struct FFilterVertex
{
	FVector4f Position;
	FVector2f UV;

	FFilterVertex()
	{}

	FFilterVertex(FVector4f InPositon, FVector2f InUV) : 
		Position(InPositon),
		UV(InUV)
	{}
};


class FFilterVertexDeclaration : public FRenderResource
{
public:
	TRefCountPtr<FRHIVertexDeclaration>	VertexDeclaration;

	virtual void InitRHI(FRHICommandListImmediate& RHICmdList)
	{
		FVertexDeclarationElementList Elements;
		uint16 Stride = sizeof(FFilterVertex);
		Elements.Add(FVertexElement(VET_Float4, 0, 0xff, 0, Stride));
		Elements.Add(FVertexElement(VET_Float2, 1, 0xff, 0, Stride));
	//	Elements.Add(FVertexElement(VET_Float4, 0, 0, 1, Stride, TEXT("Transform"))); 
		//Elements.Add(FVertexElement(1, 0, VET_Float3, 0, sizeof(FVector3f), 1, TEXT("SV_Transform")));
		Elements.Add(FVertexElement(1, 0xff, VET_Float3, 2, Stride, 1));
		VertexDeclaration = RHICreateVertexDeclaration(Elements);
	}

	//void Init()
	//{
	//	FVertexDeclarationElementList Elements;
	//	uint16 Stride = sizeof(FFilterVertex);
	//	Elements.Add(FVertexElement(VET_Float4, 0, 0, 0, Stride));
	//	Elements.Add(FVertexElement(VET_Float2, 1, 0, 0, Stride));
	//	VertexDeclaration = RHICreateVertexDeclaration(Elements);
	//}
};

class FScreenRectangleVertexBuffer : public FRenderResource
{
public:
	TRefCountPtr<FRHIBuffer> VertexBuffer;
	virtual void InitRHI(FRHICommandListImmediate& RHICmdList)
	{
		TArray<FFilterVertex> Vertices(6);

		Vertices[0].Position = FVector4f(-1, -1, 0, 1);
		Vertices[0].UV = FVector2f(0.f, 1.f);

		Vertices[1].Position = FVector4f(-1, 1, 0, 1);
		Vertices[1].UV = FVector2f(0.f, 0);

		Vertices[2].Position = FVector4f(1, 1, 0, 1);
		Vertices[2].UV = FVector2f(1.f, 0);

		Vertices[3].Position = FVector4f(1, -1, 0, 1);
		Vertices[3].UV = FVector2f(1.f, 1);

		Vertices[4].Position = FVector4f(0, 1, 0, 1);
		Vertices[4].UV = FVector2f(0.5f, 0);

		Vertices[5].Position = FVector4f(0, -1, 0, 1);
		Vertices[5].UV = FVector2f(0.5f, 1);


		VertexBuffer = RHICreateBuffer(FRHIBufferDesc(sizeof(FFilterVertex) * Vertices.Num(),
			sizeof(FFilterVertex)), TEXT("FScreenRectangleVertexBuffer"), ERHIAccess(ERHIAccess::VertexOrIndexBuffer));

		void* Data = RHILockBuffer(VertexBuffer.Get(), 0, sizeof(FFilterVertex) * Vertices.Num(), RLM_WriteOnly);
		FMemory::Memcpy(Data, Vertices.GetData(), sizeof(FFilterVertex) * Vertices.Num());

		RHIUnlockBuffer(VertexBuffer.Get());
	}
	
};

class FScreenRectangleIndexBuffer : public FRenderResource
{
public:
	TRefCountPtr<FRHIBuffer> IndexBuffer;

	virtual void InitRHI(FRHICommandListImmediate& RHICmdList)
	{
		const TArray<uint16> Indices =
		{
			0, 1, 3, 1, 2, 3, // Quad
			0, 4, 3	// Triangle
		};

		IndexBuffer = RHICreateBuffer(FRHIBufferDesc(sizeof(uint16) * Indices.GetSize(),
			sizeof(uint16)), TEXT("FScreenRectangleIndexBuffer"),
			ERHIAccess::VertexOrIndexBuffer);

		void* Data = RHILockBuffer(IndexBuffer.Get(), 0, sizeof(uint16) * Indices.GetSize(), RLM_WriteOnly);
		FMemory::Memcpy(Data, Indices.GetData(), sizeof(uint16) * Indices.GetSize());

		RHIUnlockBuffer(IndexBuffer.Get());

	}

};

extern TGlobalRenderResource<FScreenRectangleVertexBuffer>	GRenctangleVertexBuffer;
extern TGlobalRenderResource<FScreenRectangleIndexBuffer> GRenctangleIndexBuffer;
extern TGlobalRenderResource<FFilterVertexDeclaration>	GFilterVertexDeclaration;