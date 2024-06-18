#pragma once
#include "Object.h"
#include "Math/MathFwd.h"
#include "Containers/Array.h"
#include "StaticMeshResources.h"
#include "SceneResourceBuilder.h"
#include "PxShape.h"
#include "PxPtr.h"



class GStaticMesh : public GObject
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API_WITH_PROPERTIES(GStaticMesh, GObject);

public:
	void BuildFromGraphSceneMeshes(const TArray<class FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh*>& GraphSceneMesh);
	void BuildFromGraphSceneMesh(const class FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh& Mesh, FStaticMeshLODResources& LODResources);

	void InitResources();
	

	void SetRenderData(TUniquePtr<class FStaticMeshRenderData>&& InRenderData);
	class FStaticMeshRenderData* GetRenderData() const { return RenderData.get(); }
										   

	const TArray<TPxPtr<physx::PxShape>>& GetShapes() const
	{
		return Shapes;
	}

private:
	TUniquePtr<class FStaticMeshRenderData>	RenderData;
	bool bRenderDataInitialized = false;
	TArray<TPxPtr<physx::PxShape>> Shapes;



};