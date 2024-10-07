#include "Framework/MeshObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CameraComponent.h"
#include "SceneResourceImporter.h"

IMPLEMENT_INTRINSIC_CLASS(EMeshObject, EEntity);


EMeshObject::EMeshObject(const FObjectInitializer& ObjectInitializer) : EEntity(ObjectInitializer)
{
	MeshComponent = CreateDefaultSubobject<GStaticMeshComponent>(TEXT("MeshComponent"));
	CameraComponent = CreateDefaultSubobject<GCameraComponent>(TEXT("CameraComponent"));
}


void EMeshObject::BeginPlay()
{
	Super::BeginPlay();
	FSceneResourceImporter Importer;
	Importer.ImportFromFile(TEXT("Meshes/Cube.fbx"));

	auto Meshes = Importer.GetAllStaticMeshes();

	GStaticMesh* NewMesh = Meshes.GetSize() ? Meshes.First() : nullptr;
	MeshComponent->SetStaticMesh(NewMesh);
}
