#pragma once
#include "RenderResource.h"
#include "PositionVertexBuffer.h"
#include "Misc/EnumClassFlags.h"

enum class EVertexStreamUsage : uint8
{
	Default = 0 << 0,
	Instancing = 1 << 0,
	Overridden = 1 << 1,
	ManualFetch = 1 << 2
};

ENUM_CLASS_FLAGS(EVertexStreamUsage);


struct FVertexStreamComponent
{
	const FVertexBuffer* VertexBuffer = nullptr;

	uint32 StreamOffset;
	uint8 Offset;
	uint8 Stride;
	
	EVertexElementType Type = VET_None;

	EVertexStreamUsage StreamUsage = EVertexStreamUsage::Default;

	FVertexStreamComponent() 
	{}

	FVertexStreamComponent(const FVertexBuffer* InVertexBuffer,
		uint32 InOffset, uint32 InStride, EVertexElementType InType,
		uint32 InStreamOffset = 0, EVertexStreamUsage Usage = EVertexStreamUsage::Default) :
		VertexBuffer(InVertexBuffer),
		StreamOffset(InStreamOffset),
		Offset((uint8)InOffset),
		Stride((uint8)InStride),
		Type(InType),
		StreamUsage(Usage)
	{
	}


};



struct FStaticMeshDataType
{
	FVertexStreamComponent PositionComponent;
	FVertexStreamComponent TangentBasisComponent[2];

	TArray<FVertexStreamComponent> TextureCoordinates;

	FVertexStreamComponent LightMapCoordinateComponent;
	FVertexStreamComponent ColorComponent;


	uint8 NumTexCoords = 0;
};


class FStaticMeshVertexBuffer : public FRenderResource
{
public:
	void Init(uint32 InNumVertices, uint32 InNumTexCoords);

	void Init(const TArray<FStaticMeshVertex>& InVertices);



	virtual void InitRHI(FRHICommandListImmediate& RHICmdList);
	

private:
	uint32 NumTexCooords;
	uint32 NumVertices;





};



struct FStaticMeshVertexBuffers
{
	FStaticMeshVertexBuffer StaticMeshVertexBuffer;
	FPositionVertexBuffer PositionVertexBuffer;

	//FORCEINLINE InitWithDummyData(FRHICommandList& RHICmdList, )


};


struct FStaticMeshLODResources : public FRefCountedObject
{
public:
	FStaticMeshVertexBuffers VertexBuffers;


	void InitResources(class GStaticMesh* Parent, int32 LODIndex);





};



class FStaticMeshRenderData
{
	float ScreenSize[8];




};