#pragma once
#include <UObject/UObject.h>
#include <Core.h>



class AActor;

class DENGINE_API UActorComponent : public UObject
{
	typedef UObject Super;
public:
	UActorComponent();
	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;

public:

public:
	bool bCanEverTick = false;

protected:


};