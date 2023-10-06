#include "TestActor.h"

ATestActor::ATestActor()
{
	rotateComponent = CreateDefaultSubObject<URotateComponent>("name");
	sceneComponent = CreateDefaultSubObject<USceneComponent>("Scene");

	rotateComponent->SetRotateOnTick({ 5, 5, -5 });

}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATestActor::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

}

void ATestActor::Destroy()
{
	Super::Destroy();

}
