#pragma once
#include "Object.h"

#include "Engine/EngineBaseTypes.h"



class EEntity : public GObject
{
	//DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GEntity, GObject);
	DECLARE_CLASS_INTINSIC_NO_CTOR(EEntity, GObject);

public:
	EEntity();

	EEntity(const FObjectInitializer& ObjectInitalizer);

	void Init();

	struct FEntityTickFunction PrimaryTick;


private:

};