#include "D3D12Scene.h"

void D3D12Scene::SetCamera(D3D12Camera camera)
{
	m_camera = camera;
}

void D3D12Scene::AddModel(D3D12Model* model)
{
	if(model)
		m_models.push_back(model);
}

void D3D12Scene::AddModels(TArray<D3D12Model*> models)
{
	models.insert(m_models.end(), models.begin(), models.end());

}

void D3D12Scene::SetModels(TArray<D3D12Model*> models)
{
	m_models = models;
}

void D3D12Scene::ResetModels()
{
	m_models.clear();
}
