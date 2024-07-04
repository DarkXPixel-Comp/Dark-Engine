#include "SceneRendering.h"
#include "ScenePrivate.h"
#include "PrimitiveSceneInfo.h"
#include "Components/StaticMeshComponent.h"





void FSceneRender::RenderTest(FRHICommandListImmediate& CmdList)
{
	for (auto& Primitive : Scene->Primitives)
	{
		((GStaticMeshComponent*)Primitive->Primitive)->GetStaticMesh();
					

	}

}