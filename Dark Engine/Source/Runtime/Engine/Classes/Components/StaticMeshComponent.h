#pragma once

#include "Components/MeshComponent.h"


class GStaticMeshComponent : public GMeshComponent
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API_WITH_PROPERTIES(GStaticMeshComponent, GMeshComponent);


public:
	void SetStaticMesh(class GStaticMesh* NewMesh);
	TObjectPtr<class GStaticMesh> GetStaticMesh() const
	{
		//return nullptr;
		return StaticMesh;
	}

private:
	TObjectPtr<class GStaticMesh> StaticMesh;





};