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

FPrimitiveSceneProxy* GStaticMeshComponent::CreateSceneProxy()
{
	if (!GetStaticMesh())
	{
		return nullptr;
	}
	if (!GetStaticMesh()->GetRenderData())
	{
		return nullptr;
	}
	if (!GetStaticMesh()->GetRenderData()->IsInitialized())
	{
		return nullptr;
	}

	if (GetStaticMesh()->GetRenderData()->LODResources.Num() == 0)
	{
		return nullptr;
	}


	return CreateStaticMeshSceneProxy();
}

FPrimitiveSceneProxy* GStaticMeshComponent::CreateStaticMeshSceneProxy()
{
	FPrimitiveSceneProxy* NewProxy = new FPrimitiveSceneProxy(this, GetName());
	NewProxy->StaticMesh = GetStaticMesh().Get();




	return NewProxy;
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
