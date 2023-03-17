#include "D3D12Utils.h"
#include "D3D12Render.h"
#include "D3D12PSO.h"
#include <Engine/public/DEngine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>



std::unordered_map<UINT, std::unique_ptr<D3D12PipelineShaderRootSignature>> D3DUtil::Pipelines;
std::unordered_map<std::string, std::unique_ptr<D3D12Mesh>> D3DUtil::m_meshes;

UINT D3DUtil::CreatePipeline(eShaderType type)
{
	auto render = static_cast<D3D12Renderer*>(GEngine.GetRenderer());

	switch (type)
	{
	case Default:
	{
		std::vector<CD3DX12_ROOT_PARAMETER1> parametrs(3);
		std::vector<D3D12_ROOT_PARAMETER1> pParametrs(parametrs.size());
		CD3DX12_DESCRIPTOR_RANGE1 cbvRange;
		cbvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

		parametrs[0].InitAsConstantBufferView(0, 0);
		parametrs[1].InitAsDescriptorTable(1, &cbvRange);
		parametrs[2].InitAsConstantBufferView(2, 0);

		for (size_t i = 0; i < parametrs.size(); i++) { pParametrs[i] = parametrs[i]; }

		D3D12PipelineShaderRootSignature* PSO = new D3D12PipelineShaderRootSignature(render->m_device.Get(),
			"shaders/VertexShader.hlsl", "shaders/PixelShader.hlsl", pParametrs);

		Pipelines.emplace(PSO->GetID(), PSO);
	}
	break;
	case Light:
		break;
	default:
		break;
	}

	return 0;
}


ID3D12Device8* D3DUtil::GetDevice()
{
	return static_cast<D3D12Renderer*>(DEngine::GetEngine()->GetRenderer())->m_device.Get();
}
ID3D12CommandQueue* D3DUtil::GetCommandQueue()
{
	return static_cast<D3D12Renderer*>(DEngine::GetEngine()->GetRenderer())->m_commandQueue.Get();
}

//std::vector<D3D12Mesh*> D3DUtil::LoadMeshes(std::string path, bool bCombineMeshes)
//{
//	return std::vector<D3D12Mesh*>();
//}


D3D12Mesh* D3DUtil::LoadMesh(std::string path)
{
	if (m_meshes.find(path) != m_meshes.end())
	{
		return m_meshes.find(path)->second.get();
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (!scene) return nullptr;

	INT64 CountVertices = 0;

	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* Mesh = scene->mMeshes[i];
		CountVertices += Mesh->mNumVertices;
	}


	std::vector<Vertex> vertices(CountVertices);
	std::vector<WORD> indices;

	int64_t Counter = 0;
	int64_t LastCountVertices = 0;

	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* Mesh = scene->mMeshes[i];
		

		for (size_t j = 0; j < Mesh->mNumVertices; j++)
		{
			Vertex vert;
			{
				XMFLOAT3 position;
				position.x = Mesh->mVertices[j].x;
				position.y = Mesh->mVertices[j].y;
				position.z = Mesh->mVertices[j].z;
				vert.position = position;
			}
			if (Mesh->HasNormals())
			{
				XMFLOAT3 normal;

				normal.x = Mesh->mNormals[j].x;
				normal.y = Mesh->mNormals[j].y;
				normal.z = Mesh->mNormals[j].z;
				vert.normal = normal;
			}
			if (Mesh->HasTextureCoords(0))
			{
				XMFLOAT2 textureCoordinate;

				textureCoordinate.x = Mesh->mTextureCoords[0][i].x;
				textureCoordinate.y = Mesh->mTextureCoords[0][i].y;
				vert.textureCoordinate = textureCoordinate;
			}
			else
			{
				vert.textureCoordinate = XMFLOAT2(0.f, 0.f);
			}
			vert.color = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
			vertices[Counter] = vert;
			Counter++;
		}


		for (size_t j = 0; j < Mesh->mNumFaces; j++)
		{
			aiFace face = Mesh->mFaces[j];

			for (size_t k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k] + LastCountVertices);
			}
		}

		LastCountVertices += Mesh->mNumVertices;

	}

	m_meshes.emplace(path, std::make_unique<D3D12Mesh>(vertices, indices));
	return m_meshes.find(path)->second.get();

}

