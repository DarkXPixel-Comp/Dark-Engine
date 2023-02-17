#pragma once
#include "D3D12.h"
#include "D3D12Mesh.h"
#include "D3D12Resource.h"
#include "D3D12Types.h"
#include "D3D12Material.h"

class D3D12Model
{
	friend class D3D12Renderer;
private:
	D3D12Mesh* m_mesh;
	D3D12Material* m_material;
	D3D12UploadBufferResource<XMMATRIX> m_cbvObject;
	D3D12UploadBufferResource<D3D12MaterialConstants> m_cbvMaterial;
	
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scaling;
public:
	D3D12Model(XMFLOAT3 pos = {0, 0, 0}, XMFLOAT3 rot = {0, 0, 0}, XMFLOAT3 sc = {1, 1, 1})
		: m_cbvObject(1, true),m_cbvMaterial(1, true), m_position(pos), m_rotation(rot), m_scaling(sc) {}
	D3D12Model(D3D12Mesh* mesh,XMFLOAT3 pos = { 0, 0, 0 }, XMFLOAT3 rot = { 0, 0, 0 }, XMFLOAT3 sc = { 1, 1, 1 })
		: m_mesh(mesh), m_cbvMaterial(1, true), m_cbvObject(1, true), m_position(pos), m_rotation(rot), m_scaling(sc) {}

	void SetPosition(XMFLOAT3);
	void SetRotation(XMFLOAT3);
	void SetScaling(XMFLOAT3);

	void SetMesh(D3D12Mesh* mesh) { m_mesh = mesh; }
	void SetMaterial(D3D12Material* material) { m_material = material; }
	void FillConstantBuffer();
};