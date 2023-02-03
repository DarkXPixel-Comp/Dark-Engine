#pragma once

#include "D3D12.h"
#include "D3D12Model.h"
#include "D3D12Camera.h"

class D3D12Scene
{
	std::vector<D3D12Model*> m_models;
	D3D12Camera* m_camera;
public:
	void SetCamera(D3D12Camera* camera);
	void AddModel(D3D12Model* model);
	void AddModels(std::vector<D3D12Model*> models);
	void SetModels(std::vector<D3D12Model*> models);
	void ResetModels();









};