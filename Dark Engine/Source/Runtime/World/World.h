#pragma once
#include <memory>
#include <Containers/Array.h>
#include <Memory/TUniquePtr.h>
#include <DirectXMath.h>
#include <Actor/Actor.h>
#include <Camera/CameraActor.h>
#include <D3D12/D3D12Scene.h>




class UCameraActor;
class AActor;

using namespace DirectX;

class AWorld
{
public:
	AWorld() {}

	void Init();
	void Update(float DeltaTime);
	void Clear();
	void Destroy();
	void FillScene(D3D12Scene* scene);


	UCameraActor* GetCamera() { return m_camera.get(); }
	AActor* CreateActor();


	template <typename T>
	AActor* CreateActor();


private:
	TArray<TUniquePtr<AActor>> m_actors;
	TUniquePtr<UCameraActor> m_camera;
};

template <typename T>
AActor* AWorld::CreateActor()
{
	auto& it = m_actors.emplace_back(std::make_unique<T>());
	it->BeginPlay();
	return it.get();
}


template <typename T>
T* Cast(AActor* actor)
{
	return reinterpret_cast<T*>(actor);
}