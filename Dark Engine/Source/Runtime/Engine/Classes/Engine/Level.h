#pragma once
#include "Object.h"
#include "Framework/Entity.h"


class GLevel : public GObject
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GLevel, GObject);

public:
	TArray<TObjectPtr<EEntity>>	Entities;

	class FWorld* OwningWorld;


};