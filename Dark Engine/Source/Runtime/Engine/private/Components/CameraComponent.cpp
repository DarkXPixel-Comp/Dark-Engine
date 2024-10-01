#include "Components/CameraComponent.h"
#include "Engine/World.h"
#include "SceneInterface.h"


IMPLEMENT_INTRINSIC_CLASS(GCameraComponent, GSceneComponent);

void GCameraComponent::RegisterComponentWithWorld(FWorld* InWorld)
{
	/*if (InWorld->Scene)
	{
		InWorld->Scene->SetCamera(this);
	}*/

	Super::RegisterComponentWithWorld(InWorld);
}

void GCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	FSceneInterface* const Scene = GetWorld()->Scene;

	if (Scene)
	{
		Scene->SetCamera(this);
	}
}


void GCameraComponent::EndPlay()
{
	Super::EndPlay();

	FSceneInterface* const Scene = GetWorld()->Scene;

	if (Scene)
	{
		Scene->SetCamera(nullptr);
	}
}

