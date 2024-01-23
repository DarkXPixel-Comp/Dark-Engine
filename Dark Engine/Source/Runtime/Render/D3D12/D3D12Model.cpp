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

	XMFLOAT3 TempRotation = m_rotation;
	TempRotation.x = XMConvertToRadians(TempRotation.x);
	TempRotation.y = XMConvertToRadians(TempRotation.y);
	TempRotation.z = XMConvertToRadians(TempRotation.z);


	XMMATRIX Translation = XMMatrixTranslationFromVector(XMLoadFloat3(&m_position));
	XMMATRIX Rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&TempRotation));
	XMMATRIX Scaling = XMMatrixScalingFromVector(XMLoadFloat3(&m_scaling));


	ModelMatrix = XMMatrixMultiply(Scaling, Rotation);
	ModelMatrix = XMMatrixMultiply(ModelMatrix, Translation);

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