#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "SceneInterface.h"
#include "extensions/PxRigidBodyExt.h"

IMPLEMENT_INTRINSIC_CLASS(GPrimitiveComponent, GSceneComponent);



GPrimitiveComponent::GPrimitiveComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

GPrimitiveComponent::~GPrimitiveComponent()
{
}


uint64 FPrimitiveSceneInfoData::NextPrimitiveId = UINT64_MAX;

void GPrimitiveComponent::SetWorldLocation(FVector NewLocation)
{
	Super::SetWorldLocation(NewLocation);

	if (!bUpdatePrimitiveSceneDescription)
	{
		bUpdatePrimitiveSceneDescription = true;
		GetWorld()->Scene->UpdatePrimitiveTransform(this);
	}

	if(!GetWorld()->IsSimulate())
	{
		physx::PxTransform Transform = RigidDynamic->getGlobalPose();
		Transform.p = physx::PxVec3(NewLocation.X, NewLocation.Y, NewLocation.Z);
		RigidDynamic->setGlobalPose(Transform);
	}
}

FMatrix GPrimitiveComponent::GetRenderMatrix() const
{
	//TODO: Create GetRenderMatrix
	return GetComponentTransform().ToMatrixWithScale();
}

void GPrimitiveComponent::CreateRenderState()
{
	Super::CreateRenderState();
	GetWorld()->Scene->AddPrimitive(this);
}

void GPrimitiveComponent::CreatePhysicState()
{
	FVector Location = GetLocation();
	FRotator Rotation = GetRotation();
	RigidDynamic = GPhysicsCore->GetPhysics()->createRigidDynamic(
		physx::PxTransform(physx::PxVec3(Location.X, Location.Y, Location.Z), physx::PxQuat(0.f, physx::PxVec3(0, 0, 1))));
	RigidDynamic->setMass(Mass);
	RigidDynamic->userData = this;

	physx::PxRigidBodyExt::updateMassAndInertia(*RigidDynamic, 10.0f);

	//RigidDynamic->setAngularVelocity(physx::PxVec3(15.f, 1.f, 7.f));
	//RigidDynamic->setMass(Mass);
	//RigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
	//RigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);
	//RigidDynamic->setSleepThreshold(99999.f);
	//RigidDynamic->setWakeCounter(99999.f);
	RigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW, true);

	GetWorld()->AddPhysicComponent(RigidDynamic);

	auto Pos = RigidDynamic->getGlobalPose();

}

