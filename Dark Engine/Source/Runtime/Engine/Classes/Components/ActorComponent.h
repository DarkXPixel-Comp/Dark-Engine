#pragma once
#include <Engine/Classes/UObject/Object.h>



class UActorComponent : UObject
{
	virtual void InitializeComponent();
	virtual void TickComponent(float DeltaTime);
	virtual void BeginPlay();
	




};