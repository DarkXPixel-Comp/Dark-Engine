#pragma once
#include "Containers/DarkString.h"
#include "Math/MathFwd.h"



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


	void SetTransform(const FMatrix& InLocalToWorld, FVector InPosition);


	class FPrimitiveSceneInfo* PrimitiveSceneInfo = nullptr;


private:
	FMatrix LocalToWorld;
	FVector Position;
	class FSceneInterface* Scene;
};


