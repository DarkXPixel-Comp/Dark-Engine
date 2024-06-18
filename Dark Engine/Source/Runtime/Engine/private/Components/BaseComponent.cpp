#include "Components/BaseComponent.h"
#include "Engine/World.h"


IMPLEMENT_INTRINSIC_CLASS(GBaseComponent, GObject);


GBaseComponent::GBaseComponent(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	Owner = GetTypedOuter<EEntity>();

	ComponentTick.bCanEverTick = false;
	ComponentTick.bStartWithTickEnable = true;
	ComponentTick.TickGroup = TG_DuringPhysics;
	ComponentTick.Target = this;







}

void GBaseComponent::BeginPlay()
{
	check(!bBeginPlay);
	bBeginPlay = true;
}

void GBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, GBaseComponentTickFunction* ThisTickFunction)
{
	check(bBeginPlay);
}

void GBaseComponent::EndPlay()
{
	check(bBeginPlay);
	bBeginPlay = false;
}

void GBaseComponent::PostInit()
{
	if (Owner)
	{
		Owner->AddOwnedComponent(this);
	}
}

void GBaseComponent::OnRegister()
{
	bRegistered = true;
}

void GBaseComponent::CreateRenderState()
{
	DE_LOG(BaseComponentLog, Log, TEXT("Created RenderState"));
}

void GBaseComponent::RegisterComponent()
{
	EEntity* CurrentOwner = GetOwner();
	FWorld* OwnerWorld = CurrentOwner ? CurrentOwner->GetWorld() : nullptr;

	if (OwnerWorld)
	{
		RegisterComponentWithWorld(OwnerWorld);
	}


}

void GBaseComponent::RegisterComponentWithWorld(FWorld* InWorld)
{
	if (InWorld == nullptr)
	{
		return;
	}


	EEntity* CurrentOwner = GetOwner();
	World = InWorld;
	ExecuteRegisterEvents();
}

void GBaseComponent::ExecuteRegisterEvents()
{
	if (!bRegistered)
	{
		OnRegister();
	}

	if (!bRenderStateCreated && World->Scene && ShouldCreateRenderState())
	{
		CreateRenderState();
		bRenderStateCreated = true;
	}

	if (!bPhysicStateCreated && World && ShouldCreatePhysicState())
	{
		CreatePhysicState();
		bPhysicStateCreated = true;
	}



}

FWorld* GBaseComponent::GetWorldForCache() const
{
	FWorld* Result = nullptr;
	EEntity* CurrentOwner = GetOwner();

	if (CurrentOwner)
	{
		Result = CurrentOwner->GetWorld();
		World = Result;
	}

	return Result;
}
