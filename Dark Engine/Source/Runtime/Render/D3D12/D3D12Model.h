#pragma once
#include "D3D12.h"
#include "D3D12Mesh.h"
#include "D3D12Resource.h"

class D3D12Model
{
private:
	D3D12Mesh* m_mesh;
	D3D12UploadBufferResource<XMMATRIX> m_cbvObject;
	
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scaling;
public:
	D3D12Model(XMFLOAT3 pos = {0, 0, 0}, XMFLOAT3 rot = {0, 0, 0}, XMFLOAT3 sc = {0, 0, 0})
		: m_cbvObject(1, true) {}
	D3D12Model(D3D12Mesh* mesh, XMFLOAT3 pos = { 0, 0, 0 }, XMFLOAT3 rot = { 0, 0, 0 }, XMFLOAT3 sc = { 0, 0, 0 })
		: m_mesh(mesh), m_cbvObject(1, true) {}

	void SetPosition(XMFLOAT3);
	void SetRotation(XMFLOAT3);
	void SetScaling(XMFLOAT3);

	void SetMesh(D3D12Mesh* mesh) { m_mesh = mesh; }
	void FillConstantBuffers(const XMMATRIX mvp);

};