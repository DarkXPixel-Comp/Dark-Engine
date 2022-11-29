#pragma once
#include <Engine/Classes/Actor/Actor.h>
#include <Engine/Classes/Camera/Camera.h>



#include <vector>

class AWorld
{
public:
	AWorld(){}

	void Initialize();
	void Update();
	void Clear();
	void Destroy();

	void SetCamera(ACameraActor* camera);
	AActor* CreateActor();

	ACameraActor* GetCamera() { return Camera; }
	std::vector<AActor*>& GetActors() { return Actors; }

private:
	std::vector<AActor*> Actors;
	ACameraActor* Camera;












};