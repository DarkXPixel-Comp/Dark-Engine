#pragma once
#include "Object.h"
#include "ObjectMacros.h"
#include "Framework/Entity.h"
#include "Engine/EngineBaseTypes.h"



class GBaseComponent : public GObject
{
	DECLARE_CLASS_INTINSIC_NO_CTOR(GBaseComponent, GObject);
public:
	GBaseComponent(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	virtual void BeginPlay();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, GBaseComponentTickFunction* ThisTickFunction);
	virtual void EndPlay();

	EEntity* GetOwner() const { return Owner; }

	void RegisterComponent();



public:
	GBaseComponentTickFunction ComponentTick;
	bool bAutoRegister = true;

private:
	EEntity* Owner;
	bool bBeginPlay = false;


};