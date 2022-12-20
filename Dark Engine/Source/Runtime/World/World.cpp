#include "World.h"
#include <Engine/public/DEngine.h>

void AWorld::Initialize()
{
	Camera = new ACameraActor;


	//Camera->AddLocation({ 10, 0, 0 });


}



void AWorld::Update()
{
	float DeltaTime = GEngine.GetDeltaTime();




	for (auto& i : Actors)
	{
		i->Update();
		i->Tick(DeltaTime);
	}

	Camera->Update();


}

AActor* AWorld::CreateActor()
{
	AActor* actor = new AActor();



	Actors.push_back(actor);



	return actor;
}
