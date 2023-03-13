#include "Actor.h"

void AActor::BeginPlay()
{
	Super::BeginPlay();
}

void AActor::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	if (m_model)
	{
		m_model->SetPosition(m_position);
		m_model->SetRotation(m_rotation);
		m_model->SetScaling(m_scale);
	}
}

void AActor::Destroy()
{
}

void AActor::SetupPlayerController(FInputCore* controller)
{

}

void AActor::SetMesh(D3D12Mesh* mesh)
{
	if (!m_model)
	{
		m_model = std::make_unique<D3D12Model>(m_position, m_rotation, m_scale);
	}
	m_model->SetMesh(mesh);
}

XMFLOAT3 AActor::GetFrontVector()
{
	XMFLOAT3 Front;
	Front.x = -(cos(XMConvertToRadians(m_rotation.z)) * cos(XMConvertToRadians(m_rotation.y)));
	Front.y = -(sin(XMConvertToRadians(m_rotation.y)));
	Front.z = sin(XMConvertToRadians(m_rotation.z)) * cos(XMConvertToRadians(m_rotation.y));

	XMVECTOR FrontVector = XMLoadFloat3(&Front);
	XMVECTOR PositionVector = XMLoadFloat3(&m_position);
	XMStoreFloat3(&Front, PositionVector + FrontVector);

	return Front;
}

XMFLOAT3 AActor::GetRightVector()
{
	XMFLOAT3 Right;
	Right.x = -cos(XMConvertToRadians(m_rotation.z + 90.f)) * cos(XMConvertToRadians(0));
	Right.y = -sin(XMConvertToRadians(0));
	Right.z = sin(XMConvertToRadians(m_rotation.z + 90.f)) * cos(XMConvertToRadians(0));

	XMVECTOR RightVector = XMLoadFloat3(&Right);
	XMVECTOR PositionVector = XMLoadFloat3(&m_position);
	XMStoreFloat3(&Right, PositionVector + RightVector);

	return Right;
}

XMFLOAT3 AActor::GetUpVector()
{
	XMFLOAT3 Up;
	Up.x = cos(XMConvertToRadians(0)) * cos(XMConvertToRadians(90.f));
	Up.y = sin(XMConvertToRadians(90.f));
	Up.z = sin(XMConvertToRadians(0)) * cos(XMConvertToRadians(90.f));

	XMVECTOR UpVector = XMLoadFloat3(&Up);
	XMVECTOR PositionVector = XMLoadFloat3(&m_position);
	XMStoreFloat3(&Up, PositionVector + UpVector);

	return Up;
}
