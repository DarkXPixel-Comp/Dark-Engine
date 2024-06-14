#include "Framework/MeshObject.h"
#include "Components/StaticMeshComponent.h"

IMPLEMENT_INTRINSIC_CLASS(EMeshObject, EEntity);


EMeshObject::EMeshObject(const FObjectInitializer& ObjectInitalizer) : EEntity(ObjectInitalizer)
{
	MeshComponent = CreateDefaultSubobject<GStaticMeshComponent>(TEXT("MeshComponent"));
}
