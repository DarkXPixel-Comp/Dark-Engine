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
    m_camera->Update(DeltaTime);
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


AActor* AWorld::CreateActor()
{
    //auto& it = m_actors.GetVector().emplace_back(std::make_unique<AActor>());

    auto& it = m_actors.Emplace(std::make_unique<AActor>());
    it->BeginPlay();
    return it.get();
}