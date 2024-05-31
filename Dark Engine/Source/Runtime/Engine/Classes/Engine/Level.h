#pragma once
#include "Object.h"
#include "Framework/Entity.h"


class GLevel : public GObject
{
	DECLARE_CLASS_INTINSIC_NO_CTOR(GLevel, GObject);


public:
	GLevel(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());



	TArray<TObjectPtr<EEntity>>	Entities;

	class FWorld* OwningWorld;


};