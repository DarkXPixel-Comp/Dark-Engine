#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"




IMPLEMENT_INTRINSIC_CLASS(GStaticMeshComponent, GMeshComponent);



void GStaticMeshComponent::SetStaticMesh(GStaticMesh* NewMesh)
{
	if (NewMesh == GetStaticMesh())
	{
		return;
	}

	StaticMesh = NewMesh;
}

void GStaticMeshComponent::CreatePhysicState()
{
	Super::CreatePhysicState();

	if(StaticMesh)
	{
		for (auto& i : StaticMesh->GetShapes())
		{
			RigidDynamic->attachShape(*i);
		}
	}
}
