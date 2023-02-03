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

void D3D12Model::FillConstantBuffers(const XMMATRIX mvp)
{
	m_cbvObject.CopyData(0, mvp);
}
