#pragma once
#include "PrimitiveSceneProxy.h"
#include "Components/PrimitiveComponent.h"



class FPrimitiveSceneInfo
{
public:
	FPrimitiveSceneProxy* Proxy;

	FPrimitiveComponentId PrimitiveComponentId;


	FPrimitiveSceneInfo(class GPrimitiveComponent* InPrimitive, class FScene* InScene) {}


};
