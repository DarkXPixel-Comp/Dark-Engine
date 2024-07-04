#include "Framework/MeshObject.h"
#include "Components/StaticMeshComponent.h"
#include "SceneResourceImporter.h"

IMPLEMENT_INTRINSIC_CLASS(EMeshObject, EEntity);


EMeshObject::EMeshObject(const FObjectInitializer& ObjectInitializer) : EEntity(ObjectInitializer)
{
	GStaticMesh* NewMesh = nullptr;
	if (!(GetFlags() & OF_ClassDefaultObject))
	{
		FSceneResourceImporter Importer;
		Importer.ImportFromFile(TEXT("Meshes/Cube.fbx"));

		auto Meshes = Importer.GetAllStaticMeshes();
		NewMesh = Meshes[0];
	}

	MeshComponent = CreateDefaultSubobject<GStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetStaticMesh(NewMesh);



	//CreateDefaultSubobject<GPrimitiveComponent>("TTT");

	TObjectPtr<EEntity> DefaultObject = EEntity::StaticClass()->ClassDefaultObject;

}
