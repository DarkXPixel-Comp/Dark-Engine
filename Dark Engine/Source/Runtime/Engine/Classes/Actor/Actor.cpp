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
	Super::BeginPlay();




}

void AActor::Update()
{
	Super::Update();


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

void AActor::SetMesh(D3D12Mesh1* mesh)
{
	m_Model = new D3D12Model(mesh, Position, Rotation, Scale);



}


