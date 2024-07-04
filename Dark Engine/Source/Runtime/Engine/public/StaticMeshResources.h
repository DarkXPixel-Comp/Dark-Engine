#pragma once
#include "RenderResource.h"
#include "PositionVertexBuffer.h"
#include "Misc/EnumClassFlags.h"
#include "RawIndexBuffer.h"

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

	void Init(const TArray<class FStaticMeshVertex>& InVertices);

	FORCEINLINE uint32 GetNumVertices() const
	{
		return NumVertices;
	}


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
	FRawStaticIndexBuffer IndexBuffer;


	void InitResources(class GStaticMesh* Parent, int32 LODIndex);


private:
	bool bInitialized = false;



};



class FStaticMeshRenderData
{
public:
	float ScreenSize[8];

	TArray<TRefCountPtr<FStaticMeshLODResources>> LODResources;
	void InitResources(GStaticMesh* Owner);




	void AllocateLODResources(uint32 NumLODs);

	bool IsInitialized() const
	{
		return bIsInitialized;
	}
	
	bool bIsInitialized = false;

};