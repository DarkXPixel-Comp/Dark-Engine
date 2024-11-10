#include "SceneResourceImporter.h"


void FSceneResourceImporter::ImportFromFile(const FString& Filename)
{
	FSceneResourceBuilder::BuildGraphScene(Filename, CurrentSceneResource);
}


TArray<TObjectPtr<GStaticMesh>> FSceneResourceImporter::GetAllStaticMeshes()
{
	TArray<TObjectPtr<GStaticMesh>> Meshes;
	if (!CurrentSceneResource.RootNode)
	{
		return Meshes;
	}
	//FGraphSceneResource::FGraphSceneNode* CurrentNode = CurrentSceneResource.RootNode.get();

	ProcessNodeStaticMesh(*CurrentSceneResource.RootNode, Meshes);

	return Meshes;
}


void FSceneResourceImporter::ProcessNodeStaticMesh(FGraphSceneResource::FGraphSceneNode& Node, TArray<TObjectPtr<GStaticMesh>>& Meshes)
{
	for (auto& i : Node.Meshes)
	{
		TObjectPtr<GStaticMesh>	StaticMesh = ProcessStaticMesh(i);
		if(StaticMesh)
		{
			Meshes.Add(StaticMesh);
		}
	}


	for (auto& i : Node.ChildNodes)
	{
		ProcessNodeStaticMesh(i, Meshes);
	}
}

TObjectPtr<GStaticMesh> FSceneResourceImporter::ProcessStaticMesh(FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh& Mesh)
{
	TObjectPtr<GStaticMesh>	NewMesh = NewObject<GStaticMesh>(nullptr, GStaticMesh::StaticClass(), OF_NoFlags);
	TArray<FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh*> Meshes;
	Meshes.Add(&Mesh);
	NewMesh->BuildFromGraphSceneMeshes(Meshes);


	return NewMesh;
}