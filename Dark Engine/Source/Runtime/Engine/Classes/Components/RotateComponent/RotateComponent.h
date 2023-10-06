#pragma once
#include "../ActorComponent/ActorComponent.h"
#include <Core.h>
#include <DirectXMath.h>

class AActor;

using namespace DirectX;

class DENGINE_API URotateComponent : public UActorComponent
{
	typedef UActorComponent Super;

public:
	URotateComponent() {}
	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;

public:
	void SetRotateOnTick(XMFLOAT3 rot) { m_rotation = rot; }
	XMFLOAT3 GetRotation() { return m_rotation; }


protected:
	XMFLOAT3 m_rotation = { 0, 0, 0 };


};