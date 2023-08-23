#pragma once
#include <Engine/Classes/UObject/UObject.h>



class AActor;

class UActorComponent : public UObject
{
	typedef UObject Super;
public:
	UActorComponent(AActor*);
	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;

public:
	AActor* GetOwner();

public:
	bool bCanEverTick = false;

protected:
	AActor* m_Owner;



};