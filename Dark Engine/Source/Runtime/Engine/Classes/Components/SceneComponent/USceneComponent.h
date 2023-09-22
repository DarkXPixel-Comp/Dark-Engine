#pragma once

#include <Components/ActorComponent/ActorComponent.h>
#include <Containers/List/TList.h>
#include <Memory/TUniquePtr.h>
#include <Render/D3D12/D3D12Model.h>
#include <Math/DarkMathUtility.h>




class USceneComponent : public UActorComponent
{
	typedef UActorComponent Super;

public:
	USceneComponent();
	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;

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
	TUniquePtr<D3D12Model> m_model;

	FVector m_position{};
	FVector m_rotation{};
	FVector m_scale{};
	
	FVector m_positionOffset{};
	FVector m_rotationOffset{};
	FVector m_scaleOffset{};

};