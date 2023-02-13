#include "World.h"



void AWorld::Init()
{
    m_camera = std::make_unique<UCameraActor>();
    m_camera->BeginPlay();
}

void AWorld::Update(float DeltaTime)
{
    for(auto& i : m_actors)
    {
        i->Update(DeltaTime);
    }
}



AActor* AWorld::CreateActor()
{   
    auto& it = m_actors.emplace_back(std::make_unique<AActor>());
    it->BeginPlay();
    return it.get();
}


void AWorld::FillScene(D3D12Scene* scene)
{
    scene->ResetModels();
    for (auto& i : m_actors)
    {
        if (i->GetModel() != nullptr)
        {
            scene->AddModel(i->GetModel());
        }
    }
    scene->SetCamera(m_camera->GetCamera());

}
