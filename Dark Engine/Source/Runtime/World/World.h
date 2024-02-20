#pragma once
#include <memory>
#include <Containers/Array.h>
#include <Memory/TUniquePtr.h>
#include <DirectXMath.h>
#include <Actor/Actor.h>
#include <Camera/CameraActor.h>
#include <D3D12/D3D12Scene.h>
#include "SceneInterface.h"


#define WORLD_API


class UCameraActor;
class AActor;




using namespace DirectX;

class FWorld
{
public:
	FSceneInterface* Scene;



};



class AWorld
{
public:
	AWorld() {}

	WORLD_API void Init();
	WORLD_API void Update(float DeltaTime);
	WORLD_API void Clear();
	WORLD_API void Destroy();
	WORLD_API void FillScene(D3D12Scene* scene);


	WORLD_API UCameraActor* GetCamera() { return m_camera.get(); }
	WORLD_API TArray<TUniquePtr<AActor>>* GetActors() { return &m_actors; }
	WORLD_API AActor* CreateActor();


	template <typename T>
	AActor* CreateActor();


private:
	TArray<TUniquePtr<AActor>> m_actors;
	TUniquePtr<UCameraActor> m_camera;
};

template <typename T>
AActor* AWorld::CreateActor()
{
	auto& it = m_actors.GetVector().emplace_back(std::make_unique<T>());
	it->BeginPlay();
	return it.get();
}


template <typename T>
T* Cast(AActor* actor)
{
	return reinterpret_cast<T*>(actor);
}