#pragma once
#include <Core/Containers/Array/Array.h>
#include <Containers/String/DarkString.h>
#include <VertexTypes.h>
#include <wtypes.h>
#include "D3D12.h"
#include "D3D12Types.h"



//#define Vertex VertexPositionNormalColorTexture

#define Vertex VertexDefault

class FRenderMesh
{

};

class /*F*/D3D12Mesh : public FRenderMesh
{
	friend class D3D12Renderer;
	ID3D12Device8* m_device;

	ID3D12Resource* m_vertexBuffer;
	ID3D12Resource* m_indexBuffer;

	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;


public:
	D3D12Mesh();
	D3D12Mesh(TArray<Vertex> vertices, TArray<WORD> indices);

	D3D12Mesh(const D3D12Mesh& m) = delete;
	D3D12Mesh(D3D12Mesh&&) = delete;


	void Init(TArray<Vertex> vertices, TArray<WORD> indices);

public:
	FString PathToMesh;



};
