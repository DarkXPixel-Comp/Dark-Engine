#include "Engine/World.h"
#include "RenderGlobals.h"


FWorld* GWorld = nullptr;



void FWorld::InitWorld()
{
	GetRenderer()->AllocateScene(this);
}