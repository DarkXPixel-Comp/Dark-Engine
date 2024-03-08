#pragma once

#include <Components/ActorComponent/ActorComponent.h>
#include <Containers/List.h>
#include <Memory/TUniquePtr.h>
#include <D3D12/D3D12Model.h>
#include <Math/Vector.h>



// Scene component - содержит указатель на экземпл€р обьекта сцены




class USceneComponent : public UActorComponent
{
	typedef UActorComponent Super;

public:
	USceneComponent();
	virtual void BeginPlay() override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

public:
	void SetPosition(FVector pos);
	void SetRotation(FVector rot);
	void SetScaling(FVector sc);

	void SetPositionOffset(FVector pos);
	void SetRotationOffset(FVector rot);
	void SetScalingOffset(FVector sc);

public:
	void SetChild(USceneComponent* component);


private:
	TList<USceneComponent*> m_childComponents;


	FVector m_position;
	FVector m_rotation;
	FVector m_scale;
	
	FVector m_positionOffset;
	FVector m_rotationOffset;
	FVector m_scaleOffset;

};