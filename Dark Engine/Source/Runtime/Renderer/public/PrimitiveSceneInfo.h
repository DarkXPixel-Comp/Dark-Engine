#pragma once
#include "PrimitiveSceneProxy.h"
#include "Components/PrimitiveComponent.h"



class FPrimitiveSceneInfo
{
public:
	FPrimitiveSceneProxy* Proxy;

	FPrimitiveComponentId PrimitiveComponentId;

	class GPrimitiveComponent* Primitive = nullptr;


	FPrimitiveSceneInfo(class GPrimitiveComponent* InPrimitive, class FScene* InScene) : Primitive(InPrimitive) {}


};
