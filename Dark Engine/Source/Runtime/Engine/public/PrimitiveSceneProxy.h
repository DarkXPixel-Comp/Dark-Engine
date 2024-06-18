#pragma once
#include "Containers/DarkString.h"



struct FPrimitiveSceneProxyDesc
{
	FPrimitiveSceneProxyDesc()
	{

	}

	FPrimitiveSceneProxyDesc(const class GPrimitiveComponent* InComponent) {}
};


class FPrimitiveSceneProxy
{
public:
	FPrimitiveSceneProxy(const class GPrimitiveComponent* InComponent, const FString& Name);
	FPrimitiveSceneProxy(const FPrimitiveSceneProxyDesc& InDesc, const FString& Name);
};


