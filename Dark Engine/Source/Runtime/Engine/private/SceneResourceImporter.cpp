#include "SceneResourceImporter.h"


void FSceneResourceImporter::ImportFromFile(const FString& Filename)
{
	FSceneResourceBuilder::BuildGraphScene(Filename, CurrentSceneResource);
}


TArray<TObjectPtr<GStaticMesh>> FSceneResourceImporter::GetAllStaticMeshes()
{
	//FGraphSceneResource::FGraphSceneNode* CurrentNode = CurrentSceneResource.RootNode.get();
	TArray<TObjectPtr<GStaticMesh>> Meshes;

	ProcessNodeStaticMesh(*CurrentSceneResource.RootNode, Meshes);

	return Meshes;
}


void FSceneResourceImporter::ProcessNodeStaticMesh(FGraphSceneResource::FGraphSceneNode& Node, TArray<TObjectPtr<GStaticMesh>>& Meshes)
{
	for (auto& i : Node.Meshes)
	{
		Meshes.Add(ProcessStaticMesh(i));
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