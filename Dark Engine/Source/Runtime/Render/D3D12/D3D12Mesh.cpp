#include "D3D12Mesh.h"
#include "D3D12Resource.h"
#include "D3D12Utils.h"
#include <ResourceUploadBatch.h>
#include <BufferHelpers.h>


FD3D12Mesh::FD3D12Mesh()
{
	m_device = D3DUtil::GetDevice();
}

FD3D12Mesh::FD3D12Mesh(TArray<Vertex> vertices, TArray<WORD> indices)
{
	m_device = D3DUtil::GetDevice();
	Init(vertices, indices);
}


void FD3D12Mesh::Init(TArray<Vertex> vertices, TArray<WORD> indices)
{
	ResourceUploadBatch upload(m_device);


	upload.Begin();
	CreateStaticBuffer(m_device, upload, vertices.data(), vertices.size(),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_vertexBuffer);

	CreateStaticBuffer(m_device, upload, indices.data(), indices.size(),
		D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_indexBuffer);
	upload.End(D3DUtil::GetCommandQueue()).wait();



	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = sizeof(Vertex) * vertices.size();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	m_indexBufferView.SizeInBytes = sizeof(WORD) * indices.size();
}
