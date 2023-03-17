#include "D3D12Model.h"

void D3D12Model::SetPosition(XMFLOAT3 pos)
{
	m_position = pos;
}

void D3D12Model::SetRotation(XMFLOAT3 rot)
{
	m_rotation = rot;
}

void D3D12Model::SetScaling(XMFLOAT3 sc)
{
	m_scaling = sc;
}

void D3D12Model::FillConstantBuffer()
{
	XMMATRIX ModelMatrix = XMMatrixIdentity();
	XMMATRIX Translation = XMMatrixTranslationFromVector(XMLoadFloat3(&m_position));
	XMMATRIX Rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotation));
	XMMATRIX Scaling = XMMatrixScalingFromVector(XMLoadFloat3(&m_scaling));

	XMMATRIX ViewMatrix = XMMatrixLookToLH(XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0));
	XMMATRIX ProjectionMatrix = XMMatrixPerspectiveFovLH(90, 16.f / 9, 0.1f, 100.f);


	ModelMatrix = XMMatrixMultiply(Scaling, Rotation);
	ModelMatrix = XMMatrixMultiply(ModelMatrix, Translation);
	/*ModelMatrix = XMMatrixMultiply(ModelMatrix, ViewMatrix);
	ModelMatrix = XMMatrixMultiply(ModelMatrix, ProjectionMatrix);*/

	m_cbvObject.CopyData(0, ModelMatrix);

	D3D12MaterialConstants material;
	if (m_material)
	{
		material.Diffuse = m_material->m_diffuseAlbedo;
		material.Frensel = m_material->m_frenselR0;
		material.MatTransform = m_material->m_matTransform;
		material.Roughness = m_material->m_roughness;

	}
	
	m_cbvMaterial.CopyData(0, material);



}