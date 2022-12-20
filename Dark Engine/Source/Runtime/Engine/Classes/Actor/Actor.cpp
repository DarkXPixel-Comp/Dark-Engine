#include "Actor.h"

AActor::AActor()
{
	visible = false;

	Position = { 0, 0, 0 };
	Rotation = { 0, 0, 0 };
	Scale = { 1, 1, 1 };

}

void AActor::BeginPlay()
{




}

void AActor::Update()
{

	if (m_Model)
	{
		m_Model->Update();



		m_Model->Position = Position;
		m_Model->Rotation = Rotation;
		m_Model->Scale = Scale;
	}

}

void AActor::Destroy()
{
}

void AActor::AddLocation(XMFLOAT3 loc)
{
	Position.x += loc.x;
	Position.y += loc.y;
	Position.z += loc.z;


}



void AActor::AddRotation(XMFLOAT3 rot)
{
	Rotation.x += rot.x;
	Rotation.y += rot.y;
	Rotation.z += rot.z;



}

void AActor::AddScaling(XMFLOAT3 scal)
{
	Scale.x += scal.x;
	Scale.y += scal.y;
	Scale.z += scal.z;


}

void AActor::AddLocation(float x, float y, float z)
{
	Position.x += x;
	Position.y += y;
	Position.z += z;
}

void AActor::SetMesh(D3D12Mesh1* mesh)
{
	m_Model = new D3D12Model(mesh, Position, Rotation, Scale);



}

void AActor::SetModel(D3D12Model* model)
{
	m_Model = model;

}


