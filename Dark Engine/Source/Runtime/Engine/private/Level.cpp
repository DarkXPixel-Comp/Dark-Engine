#include "Engine/Level.h"
#include "Class.h"


IMPLEMENT_INTRINSIC_CLASS(GLevel, GObject);

GLevel::GLevel(const FObjectInitializer& ObjectInitializer)	: 
	GObject(ObjectInitializer),
	Entities(),
	OwningWorld(nullptr)
{

}

