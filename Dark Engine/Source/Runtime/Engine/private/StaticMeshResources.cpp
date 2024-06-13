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
	VertexBuffers.StaticMeshVertexBuffer.InitRHI(CmdList);








}

void FStaticMeshRenderData::AllocateLODResources(uint32 NumLODs)
{	
	LODResources.Reserve(NumLODs);

	while (LODResources.Num() < NumLODs)
	{
		LODResources.Add(new FStaticMeshLODResources);
	}
}
