#include "StaticMeshResources.h"
#include "RHICommandList.h"



void FStaticMeshVertexBuffer::Init(uint32 InNumVertices, uint32 InNumTexCoords)
{
	NumTexCooords = InNumTexCoords;
	NumVertices = InNumVertices;








}

void FStaticMeshVertexBuffer::InitRHI(FRHICommandListImmediate& RHICmdList)
{



}

void FStaticMeshLODResources::InitResources(GStaticMesh* Parent, int32 LODIndex)
{
	FRHICommandListImmediate& CmdList = FRHICommandListExecutor::GetImmediateCommandList();
	IndexBuffer.InitRHI(CmdList);
	VertexBuffers.StaticMeshVertexBuffer.InitRHI(CmdList);
	VertexBuffers.PositionVertexBuffer.InitRHI(CmdList);

	bInitialized = true;
}

void FStaticMeshRenderData::InitResources(GStaticMesh* Owner)
{
	for (uint32 i = 0; i < LODResources.Num(); ++i)
	{
		if (LODResources[i]->VertexBuffers.StaticMeshVertexBuffer.GetNumVertices() > 0)
		{
			LODResources[i]->InitResources(Owner, i);
		}

	}

	bIsInitialized = true;
}

void FStaticMeshRenderData::AllocateLODResources(uint32 NumLODs)
{	
	LODResources.Reserve(NumLODs);

	while (LODResources.Num() < NumLODs)
	{
		LODResources.Add(new FStaticMeshLODResources);
	}
}
