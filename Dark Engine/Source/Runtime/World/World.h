#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include <Engine/Classes/Actor/Actor.h>
#include <Engine/Classes/Camera/CameraActor.h>
#include <Render/D3D12/D3D12Scene.h>



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


private:
	std::vector<std::unique_ptr<AActor>> m_actors;
	std::unique_ptr<UCameraActor> m_camera;
};