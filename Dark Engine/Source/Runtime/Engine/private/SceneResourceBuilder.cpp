#include "SceneResourceBuilder.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Logging/LogMacros.h"
#include "Misc/Paths.h"


DECLARE_LOG_CATEGORY(StaticMeshBuilder, Log);


int32 FSceneResourceBuilder::BuildGraphScene(FString Path, FGraphSceneResource& SceneGraph)
{
	Path = FString::PrintF(TEXT("%s%s"), *FPaths::EngineContentDir(), *Path);
	//const aiScene* Scene = Importer.ReadFile(!Path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes);
	//const aiScene* Scene;// = Importer.ReadFileFromMemory(nullptr, 1, 0);
	const aiScene* Scene = aiImportFile(!Path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace | aiProcess_FindInstances);
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		//aiGetErrorString();
		DE_LOG(StaticMeshBuilder, Error, TEXT("Failed to build scene resources %s - %s"), *Path, *FString(aiGetErrorString()));
		aiReleaseImport(Scene);
		return 0;
	}

	BuildNode(Scene, Scene->mRootNode, SceneGraph.AddRootNode());

	aiReleaseImport(Scene);

	return 1;
}

void FSceneResourceBuilder::BuildNode(const class aiScene* Scene, aiNode* Node, FGraphSceneResource::FGraphSceneNode* GraphNode)
{
	for (uint32 i = 0; i < Node->mNumMeshes; ++i)
	{
		aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
		BuildMesh(Mesh, &GraphNode->Meshes.Emplace());

	}
	GraphNode->Name = Node->mName.C_Str();

	

	for (uint32 i = 0; i < Node->mNumChildren; ++i)
	{
		BuildNode(Scene, Node->mChildren[i], &GraphNode->ChildNodes.Emplace());
	}
}

void FSceneResourceBuilder::BuildMesh(const aiMesh* Mesh, FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh* SceneGraphMesh)
{
	check(Mesh);
	SceneGraphMesh->Vertices.Reserve(Mesh->mNumVertices);
	SceneGraphMesh->Normals.Reserve(Mesh->mNumVertices);
	SceneGraphMesh->Faces.Reserve(Mesh->mNumFaces);
	for (uint32 i = 0; i < Mesh->mNumVertices; ++i)
	{
		FVector3f Vertex;
		Vertex.X = (Mesh->mVertices[i].x);
		Vertex.Y = (Mesh->mVertices[i].y);
		Vertex.Z = (Mesh->mVertices[i].z);
		SceneGraphMesh->Vertices.Add(Vertex);

		if (Mesh->HasNormals())
		{
			FVector3f Normal;
			Normal.X = (Mesh->mNormals[i].x);
			Normal.Y = (Mesh->mNormals[i].y);
			Normal.Z = (Mesh->mNormals[i].z);
			SceneGraphMesh->Normals.Add(Normal);
		}

		if (Mesh->HasTangentsAndBitangents())
		{
			FVector3f Tangent;
			Tangent.X = (Mesh->mTangents[i].x);
			Tangent.Y = (Mesh->mTangents[i].y);
			Tangent.Z = (Mesh->mTangents[i].z);
			SceneGraphMesh->Tangents.Add(Tangent);
		}

		if (Mesh->HasVertexColors(0))
		{
			FColor Color;
			Color.R = Mesh->mColors[0][i].r;
			Color.G = Mesh->mColors[0][i].g;
			Color.B = Mesh->mColors[0][i].b;
			Color.A = Mesh->mColors[0][i].a;
			SceneGraphMesh->VertexColors.Add(Color);
		}

		for (int32 j = 0; j < Mesh->GetNumUVChannels(); ++j)
		{
			SceneGraphMesh->TexCoords[j].Reserve(Mesh->mNumVertices);
			if (Mesh->HasTextureCoords(j))
			{
				FVector2f UV;
				UV.X = (Mesh->mTextureCoords[j][i].x);
				UV.Y = (Mesh->mTextureCoords[j][i].y);
				SceneGraphMesh->TexCoords[j].Add(UV);
			}
			else
			{
				SceneGraphMesh->TexCoords[j].Add(FVector2f(0.0, 0.0));
			}
		}
	}

	SceneGraphMesh->MaterialIndex = Mesh->mMaterialIndex;
	SceneGraphMesh->MeshName = Mesh->mName.C_Str();

	for (uint32 i = 0; i < Mesh->mNumFaces; ++i)
	{
		aiFace Face = Mesh->mFaces[i];
		SceneGraphMesh->Faces.Emplace().Indices.Reserve(Face.mNumIndices);
		for (uint32 j = 0; j < Face.mNumIndices; ++j)
		{
			SceneGraphMesh->Faces[i].Indices.Add(Face.mIndices[j]);
		}
	}
}
