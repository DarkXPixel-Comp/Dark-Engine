#pragma once
#include "StaticMeshResources.h"
//#include "Engine/StaticMesh.h"
#include "Memory/TUniquePtr.h"


struct FGraphSceneResource
{
	struct FGraphSceneNode
	{
		struct FGraphSceneMesh
		{
			struct FGraphSceneFace
			{
				TArray<int32> Indices;
			};
			TArray<FVector3f> Vertices;
			TArray<FVector3f> Normals;
			TArray<FVector3f> Tangents;
			TArray<FColor> VertexColors;
			TArray<FGraphSceneFace>	Faces;
			TArray<FVector2f> TexCoords[8];
			uint32 MaterialIndex;
			FString MeshName;
			const bool bForceTriangle = true;

			uint32 GetNumUVChannels() const
			{
				uint32 Result = 0;
				for (uint32 i = 0; i < DE_ARRAY_COUNT(TexCoords); ++i)
				{
					if (TexCoords[i].Num() == 0)
					{
						return Result;
					}
					++Result;
				}
				return Result;
			}

		};
		TArray<FGraphSceneMesh>	Meshes;
		FString Name;
		TArray<FGraphSceneNode>	ChildNodes;
	};
	
	TUniquePtr<FGraphSceneNode> RootNode = nullptr;

	FGraphSceneNode* AddRootNode()
	{
		if (!RootNode)
		{
			RootNode = MakeUnique(new FGraphSceneNode());
		}
		return RootNode.get();
	}


};



class FSceneResourceBuilder
{
public:
	static void BuildGraphScene(FString Path, FGraphSceneResource& OutGraphScene);

private:
	static void BuildNode(const class aiScene* Scene, class aiNode* Node, FGraphSceneResource::FGraphSceneNode* SceneGraph);
	static void BuildMesh(const class aiMesh* Mesh, FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh* SceneGraphMesh);
};