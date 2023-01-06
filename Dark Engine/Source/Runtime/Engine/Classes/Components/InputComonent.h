#pragma once
#include <Engine/Classes/Components/ActorComponent.h>
#include <Core/Core.h>

DECLARE_MULTICAST_DELEGATE(FActionDelegate);


struct FInputActionBinding
{
	FActionDelegate ActionDelegate;





};


enum EEventType
{


};



class UInputComponent : public UActorComponent
{
	typedef UActorComponent Super;

public:
	void InitializeComponent() override;
	void TickComponent(float DeltaTime) override;
	void BeginPlay() override;






	
	void BindAxis();
	void BindAction();












};