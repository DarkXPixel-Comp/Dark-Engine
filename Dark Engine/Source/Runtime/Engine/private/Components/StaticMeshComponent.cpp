#include "Components/StaticMeshComponent.h"




IMPLEMENT_INTRINSIC_CLASS(GStaticMeshComponent, GMeshComponent);



void GStaticMeshComponent::SetStaticMesh(GStaticMesh* NewMesh)
{
	if (NewMesh == GetStaticMesh())
	{
		return;
	}

	StaticMesh = NewMesh;
}