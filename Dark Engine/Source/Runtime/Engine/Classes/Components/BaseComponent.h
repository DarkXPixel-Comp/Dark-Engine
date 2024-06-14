#pragma once
#include "Object.h"
#include "ObjectMacros.h"
#include "Framework/Entity.h"
#include "Engine/EngineBaseTypes.h"
#include "Logging/LogMacros.h"



DECLARE_LOG_CATEGORY(BaseComponentLog, Log);




class GBaseComponent : public GObject
{
	DECLARE_CLASS_INTINSIC_NO_CTOR(GBaseComponent, GObject);
public:
	GBaseComponent(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	virtual void BeginPlay();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, GBaseComponentTickFunction* ThisTickFunction);
	virtual void EndPlay();

	virtual void PostInit() override;

	virtual void OnRegister();

	virtual void CreateRenderState();

	FORCEINLINE bool IsRegistered() const { return bRegistered; }

	EEntity* GetOwner() const { return Owner; }

	void RegisterComponent();

	void RegisterComponentWithWorld(class FWorld* InWorld);

	virtual void ExecuteRegisterEvents();

	constexpr virtual bool ShouldCreateRenderState() const { return false; }


	class FWorld* GetWorld() const { return World ? World : GetWorldForCache(); }


private:
	class FWorld* GetWorldForCache() const;
	

public:
	GBaseComponentTickFunction ComponentTick;
	bool bAutoRegister = true;
	

private:
	EEntity* Owner;
	mutable class FWorld* World;
	bool bBeginPlay = false;
	bool bRegistered = false;
	bool bRenderStateCreated = false;


};