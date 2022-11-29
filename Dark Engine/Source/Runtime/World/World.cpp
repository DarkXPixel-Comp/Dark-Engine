#include "World.h"

void AWorld::Initialize()
{
	Camera = new ACameraActor;


	//Camera->AddLocation({ 10, 0, 0 });


}



void AWorld::Update()
{
	Camera->Update();

	for (auto& i : Actors)
	{
		i->Update();

	}



}

AActor* AWorld::CreateActor()
{
	AActor* actor = new AActor();



	Actors.push_back(actor);



	return actor;
}
