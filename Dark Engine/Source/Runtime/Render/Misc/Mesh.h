#pragma once
#include <Source/Runtime/Core/CoreMinimal.h>
#include <Source/Runtime/Engine/public/DEngine.h>
#include "../D3D12/D3D12.h"
#include "../D3D12/D3D12Render.h"






typedef std::vector<VertexPositionNormalColorTexture> VertexesArray;
typedef std::vector<WORD> IndexesArray;


using std::string;


class FMesh
{
	D3D12Renderer* Renderer;

	std::string name;

	VertexesArray Vertices;
	IndexesArray Indices;

	XMVECTOR WorldPosition;
	XMVECTOR LocalPosition;
	XMVECTOR Rotation;
	XMVECTOR Scale;


	Resource VertexBuffer;
	Resource IndexBuffer;

	bool IsEmpty = true;
	bool IsRendering = false;


	ComPtr<ID3D12RootSignature> RootSignature;
	ComPtr<ID3D12PipelineState> PipelineState;


public:
	FMesh();
	FMesh(VertexesArray, IndexesArray, XMVECTOR, XMVECTOR, XMVECTOR, XMVECTOR, string);

	void SetVertexesIndexesArray(VertexesArray, IndexesArray);
	void SetLocalPosition(XMVECTOR);
	void SetWorldPosition(XMVECTOR);
	void SetRotation(XMVECTOR);
	void SetScaling(XMVECTOR);

	void Draw();
	void Update();
};