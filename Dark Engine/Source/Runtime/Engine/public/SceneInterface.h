#pragma once


class FScene;

class FSceneInterface
{
public:
	virtual FScene* GetRenderScene() = 0;

	virtual void AddPrimitive(class GPrimitiveComponent* Primitive) = 0;

	virtual bool RequestUniformBufferUpdate(class FPrimitiveSceneInfo& PrimitiveSceneInfo) { return false; }




};