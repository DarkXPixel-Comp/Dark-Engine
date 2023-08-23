#pragma once
#include <vector>
#include <string>
#include <VertexTypes.h>
#include <wtypes.h>
#include "D3D12.h"
#include "D3D12Types.h"



//#define Vertex VertexPositionNormalColorTexture

#define Vertex VertexDefault

class D3D12Mesh
{
	friend class D3D12Renderer;
	ID3D12Device8* m_device;

	ID3D12Resource* m_vertexBuffer;
	ID3D12Resource* m_indexBuffer;

	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;


public:
	D3D12Mesh();
	D3D12Mesh(std::vector<Vertex> vertices, std::vector<WORD> indices);

	D3D12Mesh(const D3D12Mesh& m) = delete;
	D3D12Mesh(D3D12Mesh&&) = delete;


	void Init(std::vector<Vertex> vertices, std::vector<WORD> indices);

public:
	std::string PathToMesh;



};
