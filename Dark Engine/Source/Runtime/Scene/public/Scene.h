#pragma once
#include <Containers/Array/Array.h>
#include <UObject/UObject.h>


class USceneObject : public UObject
{

};

class FMesh
{

};


class UScene : public UObject
{
public:
	//UObject(FObjectConstructor constructor) {}
	UScene() {};
	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;


private:
	TArray<USceneObject*> usingObjects;
	TArray<USceneObject*> allObjects;

	USceneObject* CreateSceneObject();
	void UpdateSceneObject(USceneObject*);
	FMesh CreateMesh();







};