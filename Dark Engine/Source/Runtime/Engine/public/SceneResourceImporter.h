#pragma once
#include "Containers/DarkString.h"
#include "Containers/Array.h"
#include "Object.h"
#include "SceneResourceBuilder.h"
#include "Engine/StaticMesh.h"


class FSceneResourceImporter
{
public:
	void ImportFromFile(const FString& Filename);


	TArray<TObjectPtr<GStaticMesh>>	GetAllStaticMeshes();

private:
	TObjectPtr<GStaticMesh>	ProcessStaticMesh(FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh& Mesh);
	void ProcessNodeStaticMesh(FGraphSceneResource::FGraphSceneNode& Node, TArray<TObjectPtr<GStaticMesh>>& Meshes);


private:



	FGraphSceneResource CurrentSceneResource;
};