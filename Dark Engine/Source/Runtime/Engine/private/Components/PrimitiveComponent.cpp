#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "SceneInterface.h"

IMPLEMENT_INTRINSIC_CLASS(GPrimitiveComponent, GSceneComponent);



GPrimitiveComponent::GPrimitiveComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

GPrimitiveComponent::~GPrimitiveComponent()
{
}

void GPrimitiveComponent::CreateRenderState()
{
	Super::CreateRenderState();
	GetWorld()->Scene->AddPrimitive(this);
	



}

