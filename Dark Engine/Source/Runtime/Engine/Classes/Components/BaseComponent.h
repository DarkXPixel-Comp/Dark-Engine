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



public:
	GBaseComponentTickFunction ComponentTick;

private:
	EEntity* Owner;
	bool bBeginPlay = false;


};