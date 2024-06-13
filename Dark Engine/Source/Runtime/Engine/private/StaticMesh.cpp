#include "Engine/StaticMesh.h"
#include "SceneResourceBuilder.h"
#include "PositionVertexBuffer.h"



void GStaticMesh::BuildFromGraphSceneMeshes(const TArray<FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh*>& GraphSceneMesh)
{
	const uint32 NumLODs = 1;

	if (RenderData)
	{
		//ReleaseResources();
		SetRenderData(nullptr);
	}


	SetRenderData(MakeUnique<FStaticMeshRenderData>());
	GetRenderData()->AllocateLODResources(1);

	auto& LODResourcesArray = GetRenderData()->LODResources;


	for (uint32 LODIndex = 0; LODIndex < NumLODs; ++LODIndex)
	{
		FStaticMeshLODResources& LODResources = *GetRenderData()->LODResources[LODIndex].Get();

		BuildFromGraphSceneMesh(*GraphSceneMesh[LODIndex], LODResources);
	}

	
	


}

void GStaticMesh::BuildFromGraphSceneMesh(const FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh& Mesh, FStaticMeshLODResources& LODResources)
{
	int32 NumVertices = Mesh.Vertices.Num();
	uint32 NumTriangles = Mesh.bForceTriangle ?  Mesh.Faces.Num() : 0;
	

	TArray<FStaticMeshBuildVertex> StaticMeshBuildVertices;
	StaticMeshBuildVertices.Resize(NumVertices);

	for (uint32 i = 0; i < NumVertices; ++i)
	{
		FStaticMeshBuildVertex& StaticMeshVertex = StaticMeshBuildVertices[i];

		StaticMeshVertex.Position = Mesh.Vertices[i];
		StaticMeshVertex.TangentX = Mesh.Tangents[i];
		StaticMeshVertex.TangentY = FVector3f::CrossProduct(Mesh.Normals[i], Mesh.Tangents[i]);
		StaticMeshVertex.TangentZ = Mesh.Normals[i];


		for (uint32 UVIndex = 0; UVIndex < Mesh.GetNumUVChannels(); ++UVIndex)
		{
			StaticMeshVertex.UV[UVIndex] = Mesh.TexCoords[UVIndex][i];
		}
	}

	if (Mesh.VertexColors.Num())
	{
		for (uint32 i = 0; i < NumVertices; ++i)
		{
			FStaticMeshBuildVertex& StaticMeshVertex = StaticMeshBuildVertices[i];
			StaticMeshVertex.Color = Mesh.VertexColors[i];
		}
	}

	LODResources.VertexBuffers.PositionVertexBuffer.Init(StaticMeshBuildVertices);

	TArray<uint32> Indices(ArrayReserve(NumTriangles * 3));

	uint32 MinVertexIndex = MAX_uint32;
	uint16 Stride = 2;
	
	for (uint32 i = 0; i < NumTriangles; ++i)
	{
		for (uint32 j = 0; j < 3; ++j)
		{
			uint32 VertexIndex = Mesh.Faces[i].Indices[j];
			MinVertexIndex = FMath::Min(MinVertexIndex, VertexIndex);
			Indices.Add(VertexIndex);
		}
	}
	if (MinVertexIndex > MAX_uint16)
	{
		Stride = 4;
	}

	
	LODResources.IndexBuffer.SetIndices(Indices, Stride);
}

void GStaticMesh::SetRenderData(TUniquePtr<FStaticMeshRenderData>&& InRenderData)
{
	RenderData = std::move(InRenderData);
}
