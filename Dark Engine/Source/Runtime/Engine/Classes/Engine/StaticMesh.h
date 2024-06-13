#pragma once
#include "Object.h"
#include "Math/MathFwd.h"
#include "Containers/Array.h"
#include "StaticMeshResources.h"
#include "SceneResourceBuilder.h"



class GStaticMesh : public GObject
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API_WITH_PROPERTIES(GStaticMesh, GObject);

public:
	void BuildFromGraphSceneMeshes(const TArray<class FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh*>& GraphSceneMesh);
	void BuildFromGraphSceneMesh(const class FGraphSceneResource::FGraphSceneNode::FGraphSceneMesh& Mesh, FStaticMeshLODResources& LODResources);

	void SetRenderData(TUniquePtr<class FStaticMeshRenderData>&& InRenderData);
	class FStaticMeshRenderData* GetRenderData() const { return RenderData.get(); }
										   


private:
	TUniquePtr<class FStaticMeshRenderData>	RenderData;



};